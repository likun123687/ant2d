#include <gfx/camera.h>
#include <utils/debug.h>

namespace ant2d {

void Mat3::Init(float x, float y, float angle, float sx, float sy)
{
    auto c = static_cast<float>(std::cos(angle));
    auto s = static_cast<float>(std::sin(angle));

    data[0] = c * sx;
    data[1] = s * sx;
    data[3] = -s * sy;
    data[4] = +c * sy;
    data[6] = x;
    data[7] = y;
    data[2] = 0;
    data[5] = 0;
    data[8] = 1.0;
}

std::tuple<float, float> Mat3::TransformCoord(float x, float y)
{
    auto x1 = data[0] * x + data[3] * y + data[6];
    auto y1 = data[1] * x + data[4] * y + data[7];
    return std::make_tuple(x1, y1);
}

std::tuple<float, float> Mat3::TransformNormal(float x, float y)
{
    auto x1 = data[0] * x + data[3] * y;
    auto y1 = data[1] * x + data[4] * y;
    return std::make_tuple(x1, y1);
}

Camera::Camera():eye_{0, 0, 0},bound_{0.0f,0.0f,0.0f,0.0f},
    mat_{0.0f, 0.0f, 0.0f, 0.0f,0.0f},view_{0.0f, 0.0f, 0.0f, {0.0f, 0.0f}, {0.0f, 0.0f}},
    follow_(Ghost),desire_{0.0f, 0.0f},screen_{0.0f, 0.0f}
{}

void Camera::Init()
{
    float max = std::numeric_limits<float>::max();
    float min = std::numeric_limits<float>::min();
    SetBound(min, max, max, min);
    mat_.sx = 1;
    mat_.sy = 1;
}

std::tuple<float, float, float, float> Camera::P()
{
    auto hx = view_.w * mat_.sx / 2;
    auto hy = view_.h * mat_.sy / 2;
    auto left = mat_.x - hx;
    auto right = mat_.x + hx;
    auto bottom = mat_.y - hy;
    auto top = mat_.y + hy;
    return std::make_tuple(left, right, bottom, top);
}

std::tuple<float, float, float, float> Camera::GetView()
{
    return std::make_tuple(mat_.x, mat_.y, view_.w, view_.h);
}

std::tuple<float, float, float, float> Camera::Bounding()
{
    return std::make_tuple(bound_.left, bound_.top, bound_.right, bound_.bottom);
}

std::tuple<float, float> Camera::Screen2Scene(float x, float y)
{
    auto x1 = mat_.x - view_.w / 2 + x * view_.scale[0];
    auto y1 = mat_.y + view_.h / 2 - y * view_.scale[1];
    return std::make_tuple(x1, y1);
}

std::tuple<float, float> Camera::Scene2Screen(float x, float y)
{
    auto x1 = (x + view_.w / 2 - mat_.x) * view_.inv_scale[0];
    auto y1 = -(y - view_.h / 2 - mat_.y) * view_.inv_scale[1];
    return std::make_tuple(x1, y1);
}

void Camera::SetFollow(Entity entity)
{
    follow_ = entity;
}

Entity Camera::GetFollow()
{
    return follow_;
}

std::tuple<float, float> Camera::Position()
{
    return std::make_tuple(mat_.x, mat_.y);
}

void Camera::MoveTo(float x, float y)
{
    mat_.x = x;
    mat_.y = y;
    Clamp();
}

void Camera::MoveBy(float dx, float dy)
{
    mat_.x += dx;
    mat_.y += dy;
    Clamp();
}

std::tuple<float, float> Camera::Scale()
{
    return std::make_tuple(mat_.sx, mat_.sy);
}

void Camera::ScaleTo(float sx, float sy)
{
    mat_.sx = sx;
    mat_.sy = sy;
}

void Camera::ScaleBy(float dsx, float dsy)
{
    mat_.sx += dsx;
    mat_.sy += dsy;
}

float Camera::Rotation()
{
    return mat_.rt;
}

void Camera::RotateTo(float rt)
{
    mat_.rt = rt;
}

void Camera::RotateBy(float d)
{
    mat_.rt += d;
}

void Camera::SetBound(float left, float top, float right, float bottom)
{
    bound_.left = left;
    bound_.top = top;
    bound_.right = right;
    bound_.bottom = bottom;
    Clamp();
}

std::tuple<float, float> Camera::Screen()
{
    return std::make_tuple(screen_.w, screen_.h);
}

void Camera::SetViewPort(float w, float h)
{
    screen_.w = w;
    screen_.h = h;

    if (desire_.w != 0 || desire_.h != 0) {
        SetDesiredViewport(desire_.w, desire_.h);
    } else {
        view_.w = w;
        view_.h = h;
        view_.ratio = 1;
        view_.scale = math::Vec2(1, 1);
        view_.inv_scale = math::Vec2(1, 1);
    }
    Clamp();
}

void Camera::SetDesiredViewport(float w, float h)
{
    desire_.w = w;
    desire_.h = h;

    if (w != 0 && h != 0) {
        view_.w = w;
        view_.h = h;
        view_.ratio = w / h;
        view_.scale = math::Vec2(w / screen_.w, h / screen_.h);
        view_.inv_scale = math::Vec2(screen_.w / w, screen_.h / h);
    } else if (w == 0) {
        auto ratio = screen_.w / screen_.h;
        view_.w = ratio * h;
        view_.h = h;
        view_.ratio = ratio;

        auto scale = h / screen_.h;
        auto inv_scale = screen_.h / h;

        view_.scale = math::Vec2(scale, scale);
        view_.inv_scale = math::Vec2(inv_scale, inv_scale);
    } else if (h == 0) {
        auto ratio = screen_.w / screen_.h;
        view_.w = w;
        view_.h = w / ratio;
        view_.ratio = ratio;
        auto scale = w / screen_.w;
        auto inv_scale = screen_.w / w;
        view_.scale = math::Vec2(scale, scale);
        view_.inv_scale = math::Vec2(inv_scale, inv_scale);
    }

    Clamp();
}

void Camera::Clamp()
{
    auto left = mat_.x - view_.w / 2;
    auto right = mat_.x + view_.w / 2;
    if (left < bound_.left) {
        mat_.x += (bound_.left - left);
    } else if (right > bound_.right) {
        mat_.x += (bound_.right - right);
    }

    auto bottom = mat_.y - view_.h / 2;
    auto top = mat_.y + view_.h / 2;
    if (bottom < bound_.bottom) {
        mat_.y += bound_.bottom - bottom;
    } else if (top > bound_.top) {
        mat_.y += bound_.top - top;
    }
}

Camera::Mat Camera::GetMat()
{
    return mat_;
}

void Camera::SetMat(Camera::Mat mat)
{
    mat_ = mat;
}

bool Camera::InView(Transform* xf, math::Vec2 size, math::Vec2 gravity)
{
    auto w = view_.w * mat_.sx;
    auto h = view_.h * mat_.sy;

    Info("w-h--[{}-{}]--[{}-{}]", view_.w, mat_.sx, view_.h, mat_.sy);
    auto world = xf->GetWorld();
    Info("world--[{}-{}],{}, [{},{}]", world.scale[0],world.scale[1], world.rotation, world.position[0], world.position[1]);
    Info("size--[{}-{}]", size[0], size[1]);
    Info("gravity--[{}-{}]", gravity[0], gravity[1]);

    if (world.rotation == 0) { // happy path
        auto p = world.position;
        size[0] = size[0] * world.scale[0];
        size[1] = size[1] * world.scale[1];
        auto a = AABB { p[0] - size[0] * gravity[0], p[1] - size[1] * gravity[1], size[0], size[1] };
        auto b = AABB { mat_.x - w / 2, mat_.y - h / 2, w, h };
        return OverlapAB(&a, &b);
    } else {
        auto m = Mat3 {};
        m.Init(world.position[0], world.position[1], world.rotation, world.scale[0], world.scale[1]);
        // center and extent
        auto cx = -size[0] * gravity[0] + size[0] / 2;
        auto cy = -size[1] * gravity[1] + size[1] / 2;
        auto ex = size[0] / 2;
        auto ey = size[1] / 2;

        // transform center
        std::tie(cx, cy) = m.TransformCoord(cx, cy);

        for (auto& v : m.data) {
            if (v < 0) {
                v = -v;
            }
        }

        std::tie(ex, ey) = m.TransformNormal(ex, ey);
        auto a = AABB { cx - ex, cy - ey, ex * 2, ey * 2 };
        auto b = AABB { mat_.x - w / 2, mat_.y - h / 2, w, h };
        return OverlapAB(&a, &b);
    }
}

} // namespace ant2d

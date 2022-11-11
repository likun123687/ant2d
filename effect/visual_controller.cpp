#include <effect/visual_controller.h>
#include <utils/debug.h>

namespace ant2d {
// Visualize write the Live particles to vertex-buffer.
void VisualController::Visualize(PosTexColorVertex* buf, ITexture2D* tex, int live, bool additive)
{
    auto pos = position_;
    auto rots = rotation_;

    // compute vbo
    for (int i = 0; i < live; i++) {
        auto vi = i << 2;
        auto size = particle_size_[i];
        auto half = size / 2;

        float r = math::Clamp(color_[i][0], 0, 1);
        float g = math::Clamp(color_[i][1], 0, 1);
        float b = math::Clamp(color_[i][2], 0, 1);
        float a = math::Clamp(color_[i][3], 0, 1);

        auto rg = tex->GetRegion();

        // Transform matrix
        auto m = math::Mat3 {};
        m.InitializeScale1(pos[i][0], pos[i][1], rots[i], half, half);

        // bottom-left
        std::tie(buf[vi + 0].x, buf[vi + 0].y) = m.Transform(0, 0);
        buf[vi + 0].u = rg.x1;
        buf[vi + 0].v = rg.y1;

        // bottom-right
        std::tie(buf[vi + 1].x, buf[vi + 1].y) = m.Transform(size, 0);
        buf[vi + 1].u = rg.x2;
        buf[vi + 1].v = rg.y1;

        // top-right
        std::tie(buf[vi + 2].x, buf[vi + 2].y) = m.Transform(size, size);
        buf[vi + 2].u = rg.x2;
        buf[vi + 2].v = rg.y2;

        // top-left
        std::tie(buf[vi + 3].x, buf[vi + 3].y) = m.Transform(0, size);
        buf[vi + 3].u = rg.x1;
        buf[vi + 3].v = rg.y2;

        for (int j = 0; j < 4; j++) {
            buf[vi + j].r = r;
            buf[vi + j].g = g;
            buf[vi + j].b = b;
            buf[vi + j].a = a;
        }
    }
}

} // namespace ant2d

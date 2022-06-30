#pragma once

#include <array>
#include <gfx/data.h>
#include <math/vector.h>
#include <gfx/transform/transform.h>
#include <limits>
#include <tuple>

namespace ant2d {

enum class CameraMode {
    kPerspective,
    kOrthographic,
};

struct Mat3 {
public:
    std::array<float, 9> data;

    void Init(float x, float y, float angle, float sx, float sy);
    std::tuple<float, float> TransformCoord(float x, float y);
    std::tuple<float, float> TransformNormal(float x, float y);
};

class Camera {

    struct Mat {
        float x;
        float y;
        float sx;
        float sy;
        float rt;
    };

    struct WH {
        float w;
        float h;
    };

public:
    void Init();
    std::tuple<float, float, float, float> P();
    std::tuple<float, float, float, float> GetView();
    std::tuple<float, float, float, float> Bounding();
    std::tuple<float, float> Screen2Scene(float x, float y);
    std::tuple<float, float> Scene2Screen(float x, float y);
    void SetFollow(Entity entity);
    Entity GetFollow();
    std::tuple<float, float> Position();
    void MoveTo(float x, float y);
    void MoveBy(float x, float y);
    std::tuple<float, float> Scale();
    void ScaleTo(float sx, float sy);
    void ScaleBy(float dsx, float dsy);
    float Rotation();
    void RotateTo(float rt);
    void RotateBy(float d);
    void SetBound(float left, float top, float right, float bottom);
    std::tuple<float, float> Screen();
    void SetViewPort(float w, float h);
    void SetDesiredViewport(float w, float h);
    void Clamp();
    bool InView(Transform* xf, math::Vec2 size, math::Vec2 gravity);
    Camera();
    Mat GetMat();
    void SetMat(Mat mat);

private:
    math::Vec3 eye_;
    struct {
        float left;
        float top;
        float right;
        float bottom;
    } bound_;
    Mat mat_;

    struct {
        float w;
        float h;
        float ratio;
        math::Vec2 scale;
        math::Vec2 inv_scale;
    } view_;
    //View view_;
    Entity follow_;
    WH desire_;
    WH screen_;
};
}

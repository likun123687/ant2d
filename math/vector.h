#pragma once

#include <array>
#include <cmath>

namespace ant2d {
namespace math {
    class Vec2 {
    public:
        std::array<float, 2> data_;
        Vec2();
        Vec2(float item1, float item2);
        Vec2 Add(Vec2 v);
        Vec2 Sub(Vec2 v);
        Vec2 Mul(float c);
        float Len();
        bool IsZero();
        Vec2 Norm();
        float Dot(Vec2 v);
        float Cross(Vec2 v);
        const float& operator[](int i) const;
        float& operator[](int i);
    };

    class Vec3 {
    public:
        std::array<float, 3> data_;

        Vec3();
        Vec3(float item1, float item2, float item3);
        Vec3 Add(Vec3 v);
        Vec3 Sub(Vec3 v);
        Vec3 Mul(float c);
        const float& operator[](int i) const;
        float& operator[](int i);
    };

    class Vec4 {
    public:
        std::array<float, 4> data_;
        Vec4();
        Vec4(float item1, float item2, float item3, float item4);
        const float& operator[](int i) const;
        float& operator[](int i);
    };
} // namespace math
} // namespace ant2d

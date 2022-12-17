#pragma once

#include <array>
#include <cmath>
#include <type_traits>

namespace ant2d {
namespace math {
    class Vec2 {
    public:
        std::array<float, 2> data_;
        Vec2() = default;
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

        Vec3() = default;
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
        Vec4() = default;
        Vec4(float item1, float item2, float item3, float item4);
        const float& operator[](int i) const;
        float& operator[](int i);
        friend bool operator==(const Vec4& v1, const Vec4& v2);
    };

    static_assert(std::is_pod<Vec2>::value, "vec2 must be a POD type.");
    static_assert(std::is_pod<Vec3>::value, "vec3 must be a POD type.");
    static_assert(std::is_pod<Vec4>::value, "vec4 must be a POD type.");

} // namespace math
} // namespace ant2d

#pragma once

#include <array>
#include <cmath>

namespace ant2d {
namespace math {
    class Vec2 {
    public:
        std::array<float, 2> data_;
        Vec2()
            : data_()
        {
        }

        Vec2(float item1, float item2)
            : data_ { item1, item2 }
        {
        }

        Vec2 Add(Vec2 v)
        {
            return Vec2 { data_[0] + v.data_[0], data_[1] + v.data_[1] };
        }

        Vec2 Sub(Vec2 v)
        {
            return Vec2 { data_[0] - v.data_[0], data_[1] - v.data_[1] };
        }

        Vec2 Mul(float c)
        {
            return Vec2 { data_[0] * c, data_[1] * c };
        }

        float Len()
        {
            return float(std::sqrt(double(data_[0] * data_[0] + data_[1] * data_[1])));
        }

        bool IsZero()
        {
            return data_[0] == 0 && data_[1] == 0;
        }

        Vec2 Norm()
        {
            auto d = float(std::sqrt(double(data_[0] * data_[0] + data_[1] * data_[1])));
            return Vec2 { data_[0] / d, data_[1] / d };
        }

        float Dot(Vec2 v)
        {
            return data_[0] * v.data_[0] + data_[1] * v.data_[1];
        }

        float Cross(Vec2 v)
        {
            return data_[0] * v.data_[1] - data_[1] * v.data_[0];
        }

        const float& operator[](int i) const
        {
            return data_[i];
        }

        float& operator[](int i)
        {
            return data_[i];
        }
    };

    class Vec3 {
    public:
        std::array<float, 3> data_;

        Vec3()
            : data_()
        {
        }
        Vec3(float item1, float item2, float item3)
            : data_ { item1, item2, item3 }
        {
        }
        Vec3 Add(Vec3 v)
        {
            return Vec3 { data_[0] + v.data_[0], data_[1] + v.data_[1], data_[2] + v.data_[2] };
        }

        Vec3 Sub(Vec3 v)
        {
            return Vec3 { data_[0] - v.data_[0], data_[1] - v.data_[1], data_[2] - v.data_[2] };
        }

        Vec3 Mul(float c)
        {
            return Vec3 { data_[0] * c, data_[1] * c, data_[2] * c };
        }

        const float& operator[](int i) const
        {
            return data_[i];
        }

        float& operator[](int i)
        {
            return data_[i];
        }
    };

    class Vec4 {
    public:
        std::array<float, 4> data_;

        Vec4()
            : data_()
        {
        }

        Vec4(float item1, float item2, float item3, float item4)
            : data_ { item1, item2, item3, item4 }
        {
        }

        const float& operator[](int i) const
        {
            return data_[i];
        }

        float& operator[](int i)
        {
            return data_[i];
        }
    };
} // namespace math
} // namespace ant2d

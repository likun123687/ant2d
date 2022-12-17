
#include <math/vector.h>

namespace ant2d {
namespace math {
    // Vec2::Vec2()
    //     : data_()
    // {
    // }

    Vec2::Vec2(float item1, float item2)
        : data_ { item1, item2 }
    {
    }

    Vec2 Vec2::Add(Vec2 v)
    {
        return Vec2 { data_[0] + v.data_[0], data_[1] + v.data_[1] };
    }

    Vec2 Vec2::Sub(Vec2 v)
    {
        return Vec2 { data_[0] - v.data_[0], data_[1] - v.data_[1] };
    }

    Vec2 Vec2::Mul(float c)
    {
        return Vec2 { data_[0] * c, data_[1] * c };
    }

    float Vec2::Len()
    {
        return float(std::sqrt(double(data_[0] * data_[0] + data_[1] * data_[1])));
    }

    bool Vec2::IsZero()
    {
        return data_[0] == 0 && data_[1] == 0;
    }

    Vec2 Vec2::Norm()
    {
        auto d = float(std::sqrt(double(data_[0] * data_[0] + data_[1] * data_[1])));
        return Vec2 { data_[0] / d, data_[1] / d };
    }

    float Vec2::Dot(Vec2 v)
    {
        return data_[0] * v.data_[0] + data_[1] * v.data_[1];
    }

    float Vec2::Cross(Vec2 v)
    {
        return data_[0] * v.data_[1] - data_[1] * v.data_[0];
    }

    const float& Vec2::operator[](int i) const
    {
        return data_[i];
    }

    float& Vec2::operator[](int i)
    {
        return data_[i];
    }

    // Vec3::Vec3()
    //     : data_()
    // {
    // }

    Vec3::Vec3(float item1, float item2, float item3)
        : data_ { item1, item2, item3 }
    {
    }

    Vec3 Vec3::Add(Vec3 v)
    {
        return Vec3 { data_[0] + v.data_[0], data_[1] + v.data_[1], data_[2] + v.data_[2] };
    }

    Vec3 Vec3::Sub(Vec3 v)
    {
        return Vec3 { data_[0] - v.data_[0], data_[1] - v.data_[1], data_[2] - v.data_[2] };
    }

    Vec3 Vec3::Mul(float c)
    {
        return Vec3 { data_[0] * c, data_[1] * c, data_[2] * c };
    }

    const float& Vec3::operator[](int i) const
    {
        return data_[i];
    }

    float& Vec3::operator[](int i)
    {
        return data_[i];
    }

    // Vec4::Vec4()
    //     : data_()
    // {
    // }

    Vec4::Vec4(float item1, float item2, float item3, float item4)
        : data_ { item1, item2, item3, item4 }
    {
    }

    const float& Vec4::operator[](int i) const
    {
        return data_[i];
    }

    float& Vec4::operator[](int i)
    {
        return data_[i];
    }

    bool operator==(const Vec4& v1, const Vec4& v2)
    {
        return v1[0] == v2[0] && v1[1] == v2[1]
            && v1[2] == v2[2] && v1[3] == v2[3];
    }

} // namespace math
} // namespace ant2d

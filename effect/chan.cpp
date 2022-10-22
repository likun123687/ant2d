#include <vector>
#include <math/common_func.h>
#include "effect/common.h"

namespace ant2d {
enum class ChanType {
    ChanF32,
    ChanV2,
    ChanV4,
};

void normalize(float x, float y, std::array<float, 2>& n)
{
    auto div = math::InvSqrt(x * x + y * y);
    n[0] = x * div;
    n[1] = x * div;
}

class Channel_f32 {
private:
    std::vector<float> data_;

public:
    const float& operator[](int i) const
    {
        return data_[i];
    }

    float& operator[](int i)
    {
        return data_[i];
    }
    void SetConst(int32_t n, float v)
    {
        for (int i = 0; i < n; i++) {
            data_[i] = v;
        }
    }

    void SetRandom(int32_t n, Var v)
    {
        for (int i = 0; i < n; i++) {
            data_[i] = math::Random(v.GetBase(), v.GetBase() + v.GetVar());
        }
    }

    void Add(int32_t n, float v)
    {
        for (int i = 0; i < n; i++) {
            data_[i] += v;
        }
    }

    void Sub(int32_t n, float v)
    {
        for (int i = 0; i < n; i++) {
            data_[i] -= v;
        }
    }

    void Mul(int32_t n, float v)
    {
        for (int i = 0; i < n; i++) {
            data_[i] *= v;
        }
    }

    void Integrate(int32_t n, Channel_f32 ch1, float dt)
    {
        for (int i = 0; i < n; i++) {
            data_[i] += ch1[i] * dt;
        }
    }
};

class Channel_v2 {
private:
    std::vector<math::Vec2> data_;

public:
    const math::Vec2& operator[](int i) const
    {
        return data_[i];
    }

    math::Vec2& operator[](int i)
    {
        return data_[i];
    }

    void SetConst(int32_t n, float x, float y)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] = x;
            data_[i][1] = y;
        }
    }

    void SetRandom(int32_t n, float xlow, float xhigh, float ylow, float yhigh)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] = xlow;
            data_[i][1] = ylow;
        }
    }

    void Add(int32_t n, float x, float y)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] += x;
            data_[i][1] += y;
        }
    }

    void Integrate(int32_t n, Channel_v2 ch1, float dt)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] += ch1[i][0] * dt;
            data_[i][1] += ch1[i][1] * dt;
        }
    }

    // ch = normal * m, normal = normal_vector(x, y), m = magnitude
    void radialIntegrate(int32_t n, Channel_v2 xy, Channel_f32 m, float dt)
    {
        for (int i = 0; i < n; i++) {
            auto normal = std::array<float, 2> { 0, 0 };
            auto p = xy[i];

            if (p[0] != 0 || p[1] != 0) {
                normalize(p[0], p[1], normal);
            }

            data_[i][0] += m[i] * normal[0] * dt;
            data_[i][1] += m[i] * normal[1] * dt;
        }
    }

    // ch = tangent * m, normal = normal_vector(y, x), m = magnitude
    void tangentIntegrate(int32_t n, Channel_v2 xy, Channel_f32 m, float dt)
    {
        for (int i = 0; i < n; i++) {
            auto tangent = std::array<float, 2> { 0, 0 };
            auto p = xy[i];

            if (p[0] != 0 || p[1] != 0) {
                normalize(p[1], p[0], tangent);
            }

            data_[i][0] += m[i] * tangent[0] * dt;
            data_[i][1] += m[i] * tangent[1] * dt;
        }
    }
};

class Channel_v4 {
private:
    std::vector<math::Vec4> data_;

public:
    const math::Vec4& operator[](int i) const
    {
        return data_[i];
    }

    math::Vec4& operator[](int i)
    {
        return data_[i];
    }

    void SetConst(int32_t n, float x, float y, float z, float w)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] = x;
            data_[i][1] = y;
            data_[i][2] = z;
            data_[i][3] = w;
        }
    }

    void SetRandom(int32_t n, std::array<Var, 4>& v)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] = math::Random(v[0].GetBase(), v[0].GetBase() + v[0].GetVar());
            data_[i][1] = math::Random(v[1].GetBase(), v[1].GetBase() + v[1].GetVar());
            data_[i][2] = math::Random(v[2].GetBase(), v[2].GetBase() + v[2].GetVar());
            data_[i][3] = math::Random(v[3].GetBase(), v[3].GetBase() + v[3].GetVar());
        }
    }

    void Add(int32_t n, float x, float y, float z, float w)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] += x;
            data_[i][1] += y;
            data_[i][2] += z;
            data_[i][3] += w;
        }
    }

    void Sub(int32_t n, float x, float y, float z, float w)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] -= x;
            data_[i][1] -= y;
            data_[i][2] -= z;
            data_[i][3] -= w;
        }
    }

    void Integrate(int32_t n, Channel_v4 d, float dt)
    {
        for (int i = 0; i < n; i++) {
            data_[i][0] += d[i][0] * dt;
            data_[i][1] += d[i][1] * dt;
            data_[i][2] += d[i][2] * dt;
            data_[i][3] += d[i][3] * dt;
        }
    }
};

}
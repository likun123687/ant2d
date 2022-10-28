#include <cmath>
#include <random>
#include <math/vector.h>

namespace ant2d {
namespace math {
    constexpr double PI = 3.14159265358979323846;

    /// This is A approximate yet fast inverse square-root.
    inline float InvSqrt(float x)
    {
        auto xhalf = float(0.5) * x;
        auto i = *(int32_t*)(&x);
        i = int32_t(0x5f3759df) - int32_t(i >> 1);
        x = *(float*)(&i);
        x = x * (1.5 - (xhalf * x * x));
        return x;
    }

    inline float InvLength(float x, float y, float fail)
    {
        return 1 / float(std::sqrt(double(x * x + y * y)));
    }

    /// a faster way ?
    inline float Random(float low, float high)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(low, high);
        return dis(gen);
    }

    inline float ABS(float a)
    {
        if (a < 0) {
            return -a;
        }
        return a;
    }

    inline float Max(float a, float b)
    {
        if (a < b) {
            return b;
        }
        return a;
    }

    inline float Min(float a, float b)
    {
        if (a < b) {
            return a;
        }
        return b;
    }

    inline float Clamp(float v, float left, float right)
    {
        if (v > right) {
            return right;
        }
        if (v < left) {
            return left;
        }
        return v;
    }

    inline float Sin(float r)
    {
        return float(std::sin(r));
    }

    inline float Cos(float r)
    {
        return float(std::cos(r));
    }

    inline float Atan2(float y, float x)
    {
        return float(std::atan2(y, x));
    }

    inline float Floor(float v)
    {
        return float(std::floor(v));
    }

    inline float Ceil(float v)
    {
        return float(std::ceil(v));
    }

    // Radian converts degree to radian.
    inline float Radian(float d)
    {
        return d * PI / 180;
    }

    // Degree converts radian to degree.
    inline float Degree(float r)
    {
        return r * 180 / PI;
    }

    inline float AngleTo(Vec2 v1, Vec2 v2)
    {
        auto l1 = InvLength(v1[0], v1[1], 1);
        auto l2 = InvLength(v2[0], v2[1], 1);

        auto x1 = v1[0] * l1;
        auto y1 = v1[1] * l1;
        auto x2 = v2[0] * l2;
        auto y2 = v2[1] * l2;
        return x1 * x2 + y1 * y2;
    }

    inline float Direction(Vec2 v1, Vec2 v2)
    {
        return v1[0] * v2[1] - v1[1] * v2[0];
    }

    inline float Angle(Vec2 v)
    {
        return float(std::atan2(v[1], v[0]));
    }

    inline Vec2 Vector(float a)
    {
        return Vec2 { Cos(a), Sin(a) };
    }
}
}

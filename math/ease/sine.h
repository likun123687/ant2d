#pragma once
#include <math/common_func.h>
namespace ant2d {
namespace math {
    namespace ease {

        inline double InSine(double t)
        {
            return -1 * std::cos(t * PI / 2) + 1;
        }

        inline double OutSine(double t)
        {
            return std::sin(t * PI / 2);
        }

        inline double InOutSine(double t)
        {
            return -0.5 * (std::cos(PI * t) - 1);
        }

    }
}
}
#pragma once
#include <cmath>

namespace ant2d {
namespace math {
    namespace ease {

        inline double InCirc(double t)
        {
            return -1 * (std::sqrt(1 - t * t) - 1);
        }

        inline double OutCirc(double t)
        {
            t -= 1;
            return std::sqrt(1 - (t * t));
        }

        inline double InOutCirc(double t)
        {
            t *= 2;
            if (t < 1) {
                return -0.5 * (std::sqrt(1 - t * t) - 1);
            } else {
                t = t - 2;
                return 0.5 * (std::sqrt(1 - t * t) + 1);
            }
        }
    }
}
}
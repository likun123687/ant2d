#pragma once
#include <cmath>

namespace ant2d {
namespace math {
    namespace ease {

        inline double InExpo(double t)
        {
            if (t == 0) {
                return 0;
            } else {
                return std::pow(2, 10 * (t - 1));
            }
        }

        inline double OutExpo(double t)
        {
            if (t == 1) {
                return 1;
            } else {
                return 1 - std::pow(2, -10 * t);
            }
        }

        inline double InOutExpo(double t)
        {
            if (t == 0) {
                return 0;
            } else if (t == 1) {
                return 1;
            } else {
                if (t < 0.5) {
                    return 0.5 * std::pow(2, (20 * t) - 10);
                } else {
                    return 1 - 0.5 * std::pow(2, (-20 * t) + 10);
                }
            }
        }
    }
}
}
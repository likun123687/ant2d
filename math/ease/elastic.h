
#pragma once
#include <functional>
#include <cmath>
#include <math/common_func.h>

namespace ant2d {
namespace math {
    namespace ease {
        using Function = std::function<double(double)>;

        inline Function InElasticFunction(double period)
        {
            double p = period;
            return [p](double t) -> double {
                t -= 1;
                return -1 * (std::pow(2, 10 * t) * std::sin((t - p / 4) * (2 * PI) / p));
            };
        }

        inline double InElastic(double t)
        {
            return InElasticFunction(0.5)(t);
        }

        inline Function OutElasticFunction(double period)
        {
            double p = period;
            return [p](double t) -> double {
                return std::pow(2, -10 * t) * std::sin((t - p / 4) * (2 * PI / p)) + 1;
            };
        }

        inline double OutElastic(double t)
        {
            return OutElasticFunction(0.5)(t);
        }

        inline Function InOutElasticFunction(double period)
        {
            double p = period;
            return [p](double t) -> double {
                t *= 2;
                if (t < 1) {
                    t -= 1;
                    return -0.5 * (std::pow(2, 10 * t) * std::sin((t - p / 4) * 2 * PI / p));
                } else {
                    t -= 1;
                    return std::pow(2, -10 * t) * std::sin((t - p / 4) * 2 * PI / p) * 0.5 + 1;
                }
            };
        }

        inline double InOutElastic(double t)
        {
            return InOutElasticFunction(0.5)(t);
        }


    }
}
}
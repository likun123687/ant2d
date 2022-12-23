#pragma once
namespace ant2d {
namespace math {
    namespace ease {

        inline double InCubic(double t)
        {
            return t * t * t;
        }

        inline double OutCubic(double t)
        {
            t -= 1;
            return t * t * t + 1;
        }

        inline double InOutCubic(double t)
        {
            t *= 2;
            if (t < 1) {
                return 0.5 * t * t * t;
            } else {
                t -= 2;
                return 0.5 * (t * t * t + 2);
            }
        }

    }
}
}
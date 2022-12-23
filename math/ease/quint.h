#pragma once
namespace ant2d {
namespace math {
    namespace ease {

        inline double InQuint(double t)
        {
            return t * t * t * t * t;
        }

        inline double OutQuint(double t)
        {
            t -= 1;
            return t * t * t * t * t + 1;
        }

        inline double InOutQuint(double t)
        {
            t *= 2;
            if (t < 1) {
                return 0.5 * t * t * t * t * t;
            } else {
                t -= 2;
                return 0.5 * (t * t * t * t * t + 2);
            }
        }

    }
}
}
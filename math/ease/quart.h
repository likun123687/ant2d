#pragma once
namespace ant2d {
namespace math {
    namespace ease {
        inline double InQuart(double t)
        {
            return t * t * t * t;
        }

        inline double OutQuart(double t)
        {
            t -= 1;
            return -(t * t * t * t - 1);
        }

        inline double InOutQuart(double t)
        {
            t *= 2;
            if (t < 1) {
                return 0.5 * t * t * t * t;
            } else {
                t -= 2;
                return -0.5 * (t * t * t * t - 2);
            }
        }

    }
}
}
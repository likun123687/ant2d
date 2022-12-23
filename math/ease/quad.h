#pragma once
namespace ant2d {
namespace math {
    namespace ease {
        inline double InQuad(double t)
        {
            return t * t;
        }

        inline double OutQuad(double t)
        {
            return -t * (t - 2);
        }

        inline double InOutQuad(double t)
        {
            if (t < 0.5) {
                return 2 * t * t;
            } else {
                t = 2 * t - 1;
                return -0.5 * (t * (t - 2) - 1);
            }
        }
    }
}
}
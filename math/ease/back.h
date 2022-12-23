#pragma once
namespace ant2d {
namespace math {
    namespace ease {
        inline double Inback(double t)
        {
            double s = 1.70158;
            return t * t * ((s + 1) * t - s);
        }

        inline double OutBack(double t)
        {
            double s = 1.70158;
            t -= 1;
            return t * t * ((s + 1) * t + s) + 1;
        }

        inline double InOutBack(double t)
        {
            double s = 1.70158;
            t *= 2;
            if (t < 1) {
                s *= 1.525;
                return 0.5 * (t * t * ((s + 1) * t - s));
            } else {
                t -= 2;
                s *= 1.525;
                return 0.5 * (t * t * ((s + 1) * t + s) + 2);
            }
        }

    } // namespace ease
} // namespace math
} // namespace ant2d

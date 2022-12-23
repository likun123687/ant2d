#pragma once
namespace ant2d {
namespace math {
    namespace ease {

        inline double InBounce(double t)
        {
            return 1 - OutBounce(1 - t);
        }

        inline double OutBounce(double t)
        {
            if (t < 4 / 11.0) {
                return (121 * t * t) / 16.0;
            } else if (t < 8 / 11.0) {
                return (363 / 40.0 * t * t) - (99 / 10.0 * t) + 17 / 5.0;
            } else if (t < 9 / 10.0) {
                return (4356 / 361.0 * t * t) - (35442 / 1805.0 * t) + 16061 / 1805.0;
            } else {
                return (54 / 5.0 * t * t) - (513 / 25.0 * t) + 268 / 25.0;
            }
        }

        inline double InOutBounce(double t)
        {
            if (t < 0.5) {
                return InBounce(2 * t) * 0.5;
            } else {
                return OutBounce(2 * t - 1) * 0.5 + 0.5;
            }
        }

    } // namespace ease
} // namespace ease
} // namespace ant2d

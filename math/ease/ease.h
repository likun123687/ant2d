
#pragma once
namespace ant2d {
namespace math {
    namespace ease {
        inline double Linear(double t)
        {
            return t;
        }

        inline double InSquare(double t)
        {
            if (t < 1) {
                return 0;
            } else {
                return 1;
            }
        }

        inline double OutSquare(double t)
        {
            if (t > 0) {
                return 1;
            } else {
                return 0;
            }
        }

        inline double InOutSquare(double t)
        {
            if (t < 0.5) {
                return 0;
            } else {
                return 1;
            }
        }

    }
}
}
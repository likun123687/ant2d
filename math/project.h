#pragma once
#include <cmath>
#include <math/matrix.h>

namespace ant2d {
namespace math {
    Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);

    // Equivalent to Ortho with the near and far planes being -1 and 1, respectively
    Mat4 Ortho2D(float left, float right, float bottom, float top);

    Mat4 Perspective(float fovy, float aspect, float near, float far);

    Mat4 Frustum(float left, float right, float bottom, float top, float near, float far);
} // namespace math
} // namespace ant2d

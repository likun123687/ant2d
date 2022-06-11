#pragma once
#include <cmath>
#include <math/matrix.h>

namespace ant2d {
namespace math {

    Mat4 Ortho(float left, float right, float bottom, float top, float near, float far)
    {
        float rml = right - left;
        float tmb = top - bottom;
        float fmn = far - near;
        return Mat4 { float(2. / rml), 0, 0, 0,
            0, float(2. / tmb), 0, 0,
            0, 0, float(-2. / fmn), 0,
            float(-(right + left) / rml), float(-(top + bottom) / tmb), float(-(far + near) / fmn), 1 };
    }

    // Equivalent to Ortho with the near and far planes being -1 and 1, respectively
    Mat4 Ortho2D(float left, float right, float bottom, float top)
    {
        return Ortho(left, right, bottom, top, -1, 1);
    }

    Mat4 Perspective(float fovy, float aspect, float near, float far)
    {
        float nmf = near - far;
        float f = float(1. / std::tan(double(fovy) / 2.0));
        return Mat4 { float(f / aspect), 0, 0, 0,
            0, float(f), 0, 0, 0,
            0, float((near + far) / nmf), -1, 0,
            0, float((2. * far * near) / nmf), 0 };
    }

    Mat4 Frustum(float left, float right, float bottom, float top, float near, float far)
    {
        float rml = right - left;
        float tmb = top - bottom;
        float fmn = far - near;

        float A = (right + left) / rml;
        float B = (top + bottom) / tmb;
        float C = -(far + near) / fmn;
        float D = -(2 * far * near) / fmn;
        return Mat4 { float((2. * near) / rml), 0, 0, 0, 0, float((2. * near) / tmb), 0, 0, float(A), float(B), float(C), -1, 0, 0, float(D), 0 };
    }

} // namespace math
} // namespace ant2d

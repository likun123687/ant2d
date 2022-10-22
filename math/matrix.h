#pragma once
#include <array>
#include <math/vector.h>

namespace ant2d {
namespace math {
    class Mat3 {
    public:
        std::array<float, 9> data_;

        // SetCol sets a Column within the Matrix, so it mutates the calling matrix.
        void SetCol(int col, Vec3 v);

        // SetRow sets a Row within the Matrix, so it mutates the calling matrix.
        void SetRow(int row, Vec3 v);

        // Diag returns main diagonal (meaning all elements such that row==col).
        Vec3 Diag();

        // Transform transforms (x, y) to (x1, y).
        //
        // matrix multiplication carried out on paper:
        // |1    x| |c -s  | |sx     | | 1 ky  | |1   -ox|
        // |  1  y| |s  c  | |   sy  | |kx  1  | |  1 -oy|
        // |     1| |     1| |      1| |      1| |     1 |
        //   move    rotate    scale     skew      origin
        std::tuple<float, float> Transform(float x, float y);

        // Initialize defines a 3-D Matrix.
        //            | x |
        //            | y |
        //            | 1 |
        // | e0 e3 e6 |
        // | e1 e4 e7 |
        // | e2 e5 e8 |
        void Initialize(float x, float y, float angle,
            float sx, float sy, float ox,
            float oy, float kx, float ky);

        void InitializeScale1(float x, float y, float angle, float ox, float oy);

        // Ident3 returns the 3x3 identity matrix.
        static Mat3 Ident3();
    };

    class Mat4 {
    public:
        std::array<float, 16> data_;

        // SetCol sets a Column within the Matrix.
        void SetCol(int col, Vec4 v);
        // SetRow sets a Row within the Matrix.
        void SetRow(int row, Vec4 v);
        // Diag returns main diagonal (meaning all elements such that row==col).
        Vec4 Diag();

        // Ident4 returns the 4x4 identity matrix.
        static Mat4 Ident4();

        // At returns the matrix element at the given row and column.
        float At(int row, int col);

        // Set sets the corresponding matrix element at the given row and column.
        void Set(int row, int col, float value);
    };

    // Ident4 returns the 4x4 identity matrix.
    Mat4 Ident4()
    {
        return Mat4 { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    }
} // namespace math
} // namespace ant2d

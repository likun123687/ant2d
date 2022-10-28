#include <math/matrix.h>
#include <tuple>

namespace ant2d {
namespace math {

    Mat3::Mat3(float item1, float item2, float item3, float item4, float item5, float item6, float item7, float item8, float item9)
        : data_ { item1, item2, item3, item4, item5, item6, item7, item8, item9 }
    {
    }

    // SetCol sets a Column within the Matrix, so it mutates the calling matrix.
    void Mat3::SetCol(int col, Vec3 v)
    {
        data_[col * 3 + 0] = v[0];
        data_[col * 3 + 1] = v[1];
        data_[col * 3 + 2] = v[2];
    }

    // SetRow sets a Row within the Matrix, so it mutates the calling matrix.
    void Mat3::SetRow(int row, Vec3 v)
    {
        data_[row + 0] = v[0];
        data_[row + 3] = v[1];
        data_[row + 6] = v[2];
    }

    // Diag returns main diagonal (meaning all elements such that row==col).
    Vec3 Mat3::Diag()
    {
        return Vec3 { data_[0], data_[4], data_[8] };
    }

    // Transform transforms (x, y) to (x1, y).
    //
    // matrix multiplication carried out on paper:
    // |1    x| |c -s  | |sx     | | 1 ky  | |1   -ox|
    // |  1  y| |s  c  | |   sy  | |kx  1  | |  1 -oy|
    // |     1| |     1| |      1| |      1| |     1 |
    //   move    rotate    scale     skew      origin
    std::tuple<float, float> Mat3::Transform(float x, float y)
    {
        auto x1 = data_[0] * x + data_[3] * y + data_[6];
        auto y1 = data_[1] * x + data_[4] * y + data_[7];
        return std::make_tuple(x1, y1);
    }

    // Initialize defines a 3-D Matrix.
    //            | x |
    //            | y |
    //            | 1 |
    // | e0 e3 e6 |
    // | e1 e4 e7 |
    // | e2 e5 e8 |
    void Mat3::Initialize(float x, float y, float angle,
        float sx, float sy, float ox,
        float oy, float kx, float ky)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        data_[0] = c * sx - ky * s * sy; // = a
        data_[1] = s * sx + ky * c * sy; // = b
        data_[3] = kx * c * sx - s * sy; // = c
        data_[4] = kx * s * sx + c * sy; // = d
        data_[6] = x - ox * data_[0] - oy * data_[3];
        data_[7] = y - ox * data_[1] - oy * data_[4];
        data_[2] = 0;
        data_[5] = 0;
        data_[8] = 1.0;
    }

    void Mat3::InitializeScale1(float x, float y, float angle, float ox, float oy)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        data_[0] = c; // = a
        data_[1] = s; // = b
        data_[3] = -s; // = c
        data_[4] = +c; // = d
        data_[6] = x - ox * data_[0] - oy * data_[3];
        data_[7] = y - ox * data_[1] - oy * data_[4];
        data_[2] = 0;
        data_[5] = 0;
        data_[8] = 1.0;
    }

    // Ident3 returns the 3x3 identity matrix.
    Mat3 Mat3::Ident3()
    {
        return Mat3 { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    }

    Mat4::Mat4(float item1, float item2, float item3, float item4, float item5, float item6, float item7, float item8, float item9,
        float item10, float item11, float item12, float item13, float item14, float item15, float item16)
        : data_ { item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13, item14, item15, item16 }
    {
    }

    // SetCol sets a Column within the Matrix.
    void Mat4::SetCol(int col, Vec4 v)
    {
        data_[col * 4 + 0] = v[0];
        data_[col * 4 + 1] = v[1];
        data_[col * 4 + 2] = v[2];
        data_[col * 4 + 3] = v[3];
    }

    // SetRow sets a Row within the Matrix.
    void Mat4::SetRow(int row, Vec4 v)
    {
        data_[row + 0] = v[0];
        data_[row + 4] = v[1];
        data_[row + 8] = v[2];
        data_[row + 12] = v[3];
    }

    // Diag returns main diagonal (meaning all elements such that row==col).
    Vec4 Mat4::Diag()
    {
        return Vec4 { data_[0], data_[5], data_[10], data_[15] };
    }

    // Ident4 returns the 4x4 identity matrix.
    Mat4 Mat4::Ident4()
    {
        return Mat4 { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    }

    // At returns the matrix element at the given row and column.
    float Mat4::At(int row, int col)
    {
        return data_[col * 4 + row];
    }

    // Set sets the corresponding matrix element at the given row and column.
    void Mat4::Set(int row, int col, float value)
    {
        data_[col * 4 + row] = value;
    }

    const float& Mat4::operator[](int i) const
    {
        return data_[i];
    }

    float& Mat4::operator[](int i)
    {
        return data_[i];
    }
}
}

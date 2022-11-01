#include <vector>
#include <math/common_func.h>
#include <effect/common.h>

namespace ant2d {
enum class ChanType {
    ChanF32,
    ChanV2,
    ChanV4,
};

void normalize(float x, float y, std::array<float, 2>& n);
class Channel_f32 {
private:
    float *data_;
    int32_t size_;

public:
    Channel_f32(float *data, int32_t size);
    const float& operator[](int i) const;

    float& operator[](int i);
    void SetConst(int32_t n, float v);

    void SetRandom(int32_t n, Var v);

    void Add(int32_t n, float v);

    void Sub(int32_t n, float v);

    void Mul(int32_t n, float v);

    void Integrate(int32_t n, Channel_f32 ch1, float dt);
};

class Channel_v2 {
private:
    math::Vec2 *data_;
    int32_t size_;

public:
    Channel_v2(math::Vec2 *data, int32_t size);

    const math::Vec2& operator[](int i) const;

    math::Vec2& operator[](int i);

    void SetConst(int32_t n, float x, float y);

    void SetRandom(int32_t n, float xlow, float xhigh, float ylow, float yhigh);

    void Add(int32_t n, float x, float y);

    void Integrate(int32_t n, Channel_v2 ch1, float dt);

    // ch = normal * m, normal = normal_vector(x, y), m = magnitude
    void radialIntegrate(int32_t n, Channel_v2 xy, Channel_f32 m, float dt);

    // ch = tangent * m, normal = normal_vector(y, x), m = magnitude
    void tangentIntegrate(int32_t n, Channel_v2 xy, Channel_f32 m, float dt);
};

class Channel_v4 {
private:
    math::Vec4 *data_;
    int32_t size_;

public:
    Channel_v4(math::Vec4 *data, int32_t size);

    const math::Vec4& operator[](int i) const;
    math::Vec4& operator[](int i);

    void SetConst(int32_t n, float x, float y, float z, float w);
    void SetRandom(int32_t n, std::array<Var, 4>& v);

    void Add(int32_t n, float x, float y, float z, float w);

    void Sub(int32_t n, float x, float y, float z, float w);

    void Integrate(int32_t n, Channel_v4 d, float dt);
};

}
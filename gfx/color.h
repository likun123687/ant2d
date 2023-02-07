#pragma once
#include <cstdint>
#include <type_traits>

namespace ant2d {
namespace color {
    struct Byte4 {
        Byte4() = default;

        Byte4(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        {
            r = r_;
            g = g_;
            b = b_;
            a = a_;
        }

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    static_assert(std::is_pod<Byte4>::value, "Byte4 must be a POD type.");

} // namespace color

} // namespace ant2d

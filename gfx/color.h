#pragma once
#include <cstdint>
#include <type_traits>

namespace ant2d {
namespace color {
    struct Float4 {
        Float4() = default;
        Float4(uint32_t byte4_color)
        {
            r = static_cast<uint8_t>(byte4_color >> 24) / 255.0f;
            g = static_cast<uint8_t>(byte4_color >> 16) / 255.0f;
            b = static_cast<uint8_t>(byte4_color >> 8) / 255.0f;
            a = static_cast<uint8_t>(byte4_color >> 0) / 255.0f;
        }

        Float4(float r_, float g_, float b_, float a_)
        {
            r = r_;
            g = g_;
            b = b_;
            a = a_;
        }

        float r;
        float g;
        float b;
        float a;
    };

    static_assert(std::is_pod<Float4>::value, "float4 must be a POD type.");

} // namespace color

} // namespace ant2d

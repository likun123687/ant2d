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

        uint32_t GetByte4Color()
        {
            uint8_t r_ = r * 255.0f;
            uint8_t g_ = g * 255.0f;
            uint8_t b_ = b * 255.0f;
            uint8_t a_ = a * 255.0f;
            return (static_cast<uint32_t>(r) << 24 | static_cast<uint32_t>(g) << 16
                | static_cast<uint32_t>(b) << 8 | static_cast<uint32_t>(a) << 0);
        }

        float r;
        float g;
        float b;
        float a;
    };

    static_assert(std::is_pod<Float4>::value, "float4 must be a POD type.");

} // namespace color

} // namespace ant2d

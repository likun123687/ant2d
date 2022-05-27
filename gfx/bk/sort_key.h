#pragma once
#include <utils/debug.h>

namespace ant2d {
const uint32_t kStateLayerMask = 0xF0000000;
const uint32_t kStateShaderMask = 0x0F000000;
const uint32_t kStateBlendMask = 0x00F00000;
const uint32_t kStateTextureMask = 0x000FF000;

// SortKey FORMAT
// 64bit:
// 0000 - 0000000000 -       00000 -  000 - 0000000000
//  ^        ^                  ^      ^       ^
//  |        |                  |      |       |
//  |      z-order()         shader(2^5)   |    texture(2^10)
// Layer(2^4)                        blend(2^3)
// 4 + 10 + 5 + 3 + 10

class SortKey {
public:
    uint64_t Encode()
    {
        return 0 | static_cast<uint64_t>(layer_) << 28 | static_cast<uint64_t>(order_) << 18 | static_cast<uint64_t>(shader_) << 13 | static_cast<uint64_t>(blend_) << 10 | static_cast<uint64_t>(texture_);
    }

    void Decode(uint64_t key)
    {
        texture_ = static_cast<uint16_t>((key >> 00) & (0xFFFF >> 16 - 10));
        blend_ = static_cast<uint16_t>((key >> 10) & (0xFFFF >> 16 - 3));
        shader_ = static_cast<uint16_t>((key >> 13) & (0xFFFF >> 16 - 5));
        order_ = static_cast<uint16_t>((key >> 18) & (0xFFFF >> 16 - 10));
        layer_ = static_cast<uint16_t>((key >> 28) & (0xFFFF >> 16 - 4));
    }

public:
    uint16_t layer_;
    uint16_t order_;
    uint16_t shader_;
    uint16_t blend_;
    uint16_t texture_;
};
}

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
//  |      z-order()         pipeline(2^5)   |    texture(2^10)
// Layer(2^4)                        blend(2^3)
// 4 + 10 + 5 + 3 + 10

class SortKey {
public:
    uint64_t Encode()
    {
        return 0 | static_cast<uint64_t>(layer_) << 28 | static_cast<uint64_t>(order_) << 18 | static_cast<uint64_t>(pipeline_) << 13 | static_cast<uint64_t>(blend_) << 10 | static_cast<uint64_t>(texture_);
    }

    void Decode(uint64_t key)
    {
        texture_ = static_cast<uint16_t>((key >> 00) & ((1<<10) - 1));
        blend_ = static_cast<uint16_t>((key >> 10) & ((1<<3) - 1));
        pipeline_ = static_cast<uint16_t>((key >> 13) & ((1<<5) - 1));
        order_ = static_cast<uint16_t>((key >> 18) & ((1<<10) - 1));
        layer_ = static_cast<uint16_t>((key >> 28) & ((1<<4) - 1));
    }

public:
    uint16_t layer_;
    uint16_t order_;
    uint16_t pipeline_;
    uint16_t blend_;
    uint16_t texture_;
};
}

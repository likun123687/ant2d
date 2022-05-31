#pragma once
#include <gfx/bk/sokol_gfx_bk.h>
namespace ant2d {

struct State {
    uint64_t rgb_write;
    uint64_t alpha_write;
    uint64_t depth_write;

    uint64_t depth_test_mask;
    uint64_t depth_test_shift;

    uint64_t blend_mask;
    uint64_t blend_shift;

    uint64_t pt_mask;
    uint64_t pt_shift;
};

extern State kState;

// zero means no depth-test
struct StateDepth {
    uint64_t less;
    uint64_t lequal;
    uint64_t equal;
    uint64_t gequal;
    uint64_t greater;
    uint64_t not_equal;
    uint64_t never;
    uint64_t always;
};

extern StateDepth kStateDepth;

struct BlendFactor {
    sg_blend_factor src;
    sg_blend_factor dst;
};

extern BlendFactor kBlend[];

extern sg_compare_func kCmpFunc[];
extern sg_primitive_type kPrimInfo[];

}

#pragma once
#include <gfx/bk/res_manager.h>
#include <gfx/bk/sort_key.h>

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

// zero means no blend
struct StateBlend {
   uint64_t default_s;
   uint64_t isable;
   uint64_t alpha_premultiplied;
   uint64_t alpha_non_premultiplied;
   uint64_t additive;
};

extern StateBlend kStateBlend;

struct BlendFactor {
    sg_blend_factor src;
    sg_blend_factor dst;
};

extern BlendFactor kBlend[];

extern sg_compare_func kCmpFunc[];
extern sg_primitive_type kPrimInfo[];

struct AppState {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
};

struct Stream {
    uint16_t vertex_buffer;
};

class Rect {
public:
    uint16_t x_;
    uint16_t y_;
    uint16_t w_;
    uint16_t h_;

    Rect()
        : x_(0)
        , y_(0)
        , w_(0)
        , h_(0)
    {
    }

    Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
        : x_(x)
        , y_(y)
        , w_(w)
        , h_(h)
    {
    }

    void Clear()
    {
        x_ = 0;
        y_ = 0;
        w_ = 0;
        h_ = 0;
    }

    bool IsZero()
    {
        return x_ == 0 && y_ == 0 && w_ == 0 && h_ == 0;
    }
};

class RenderDraw {
public:
    void Reset()
    {
        index_buffer_ = 0;
        first_index_ = 0;
        num_ = 0;
        scissor_ = 0;
    }
    uint16_t index_buffer_;
    std::array<Stream, 2> vertex_buffers_;
    std::array<uint16_t, 2> textures_;

    uint16_t first_index_;
    uint16_t num_;

    uint16_t uniformblock_begin_;
    uint16_t uniformblock_end_;

    uint32_t stencil_;
    uint16_t scissor_;

    uint64_t state_;
};

class RenderContext {
private:
    ResManager* res_manager_;
    UniformblockBuffer* uniformblock_buffer_;
    Rect w_rect_;
    float pixel_ratio_;
    std::vector<Rect> clips_;
    AppState app_state_;
    sg_pipeline_desc pipeline_desc_;

    void UpdateScissor(RenderDraw& draw, RenderDraw& current_state);
    void UpdateStencil(RenderDraw& draw, RenderDraw& current_state);
    void UpdateStateBind(RenderDraw& draw, RenderDraw& current_state);
    void UpdateProgram(RenderDraw& draw, RenderDraw& current_state, SortKey& sort_key, uint16_t& shader_id, bool& program_changed);
    void UpdateUniformblockBind(RenderDraw& draw, RenderDraw& current_state);
    void UpdateTextureBind(RenderDraw& draw, RenderDraw& current_state, bool program_changed);
    void UpdateBufferBind(RenderDraw& draw, RenderDraw& current_state, uint16_t shader_id);
    void DoDraw(RenderDraw& draw, RenderDraw& current_state);

public:
    RenderContext(ResManager* res_manager, UniformblockBuffer* uniformblock_buffer);
    void Reset();
    uint16_t AddClipRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void Draw(std::vector<uint64_t> sort_keys, std::vector<uint16_t> sort_values,
        std::vector<RenderDraw> draw_list);

    void BindUniformblock(uint32_t begin, uint32_t end);
    void BindState(uint64_t changed_flags, uint64_t new_flags);
    void SetwRect(uint16_t w, uint16_t h);
    void SetPixelRatio(float pixel_ratio);
    void BindAttributes(Shader* sh, RenderDraw* draw);
};
}

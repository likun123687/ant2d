#pragma once
#include <gfx/bk/res_manager.h>
#include <gfx/bk/sort_key.h>

namespace ant2d {

struct BkState {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
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
    RenderDraw()
        : index_buffer_(0)
        , vertex_buffers_()
        , textures_()
        , first_index_(0)
        , num_(0)
        , uniformblock_begin_(0)
        , uniformblock_end_(0)
        , scissor_(0)
    {
    }
    void Reset()
    {
        index_buffer_ = 0;
        first_index_ = 0;
        num_ = 0;
        scissor_ = 0;
    }
    uint16_t index_buffer_;
    std::array<uint16_t, 2> vertex_buffers_;
    std::array<uint16_t, 2> textures_;

    uint16_t first_index_;
    uint16_t num_;

    uint16_t uniformblock_begin_;
    uint16_t uniformblock_end_;

    uint16_t scissor_;
};

class RenderContext {
private:
    ResManager* res_manager_;
    UniformblockBuffer* uniformblock_buffer_;
    Rect w_rect_;
    float pixel_ratio_;
    std::vector<Rect> clips_;
    BkState bk_state_;

public:
    RenderContext(ResManager* res_manager, UniformblockBuffer* uniformblock_buffer);
    void Reset();
    void Init();
    uint16_t AddClipRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void UpdateScissor(RenderDraw& draw, RenderDraw& current_state);
    void UpdateUniformblockBind(RenderDraw& draw, RenderDraw& current_state);
    void UpdateTextureBind(RenderDraw& draw, RenderDraw& current_state);
    void UpdateBufferBind(RenderDraw& draw, RenderDraw& current_state);
    void UpdatePipeline(uint16_t& old_pipeline, uint16_t& new_pipeline);
    void DoDraw(RenderDraw& draw, RenderDraw& current_state);

    void Draw(std::vector<uint64_t> sort_keys, std::vector<uint16_t> sort_values,
        std::vector<RenderDraw> draw_list);

    void BindUniformblock(uint32_t begin, uint32_t end);
    void BindState(uint64_t changed_flags, uint64_t new_flags);
    void SetwRect(uint16_t w, uint16_t h);
    void SetPixelRatio(float pixel_ratio);
    void BindAttributes(Shader* sh, RenderDraw* draw);
};
}

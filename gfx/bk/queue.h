#pragma once

#include <gfx/bk/render_context.h>
#include <gfx/bk/res_manager.h>
#include <gfx/bk/sort_key.h>
#include <gfx/bk/uniformblock.h>
#include <utils/singleton.h>

#include <array>
#include <cstdint>

namespace ant2d {

enum class SortMode {
    kSequential,
    kAscending,
    kDescending,
};

const uint16_t kMaxQueueSize = 8 << 10;

class RenderQueue {
private:
    SortMode sort_mode_;
    std::array<uint64_t, kMaxQueueSize> sort_keys_;
    std::array<uint16_t, kMaxQueueSize> sort_values_;
    std::array<RenderDraw, kMaxQueueSize> draw_call_list_;
    uint16_t draw_call_num_;
    SortKey sort_key_;
    RenderDraw draw_call_;
    uint16_t uniformblock_begin_;
    uint16_t uniformblock_end_;

    std::array<Rect, 4> viewports_;
    std::array<Rect, 4> scissors_;

    ResManager* rm_;
    std::unique_ptr<UniformblockBuffer> uniformblock_buffer_;
    std::unique_ptr<RenderContext> ctx_;

public:
    RenderQueue();
    void Init();
    void Reset(uint16_t w, uint16_t h, float pr);
    //void SetState(uint64_t state, uint32_t rgba);
    void SetIndexBuffer(uint16_t id, uint16_t first_index, uint16_t num);
    void SetVertexBuffer(uint8_t stream, uint16_t id);
    void SetTexture(uint8_t stage, uint16_t tex_id);
    void SetUniformblock(uint16_t id, uint8_t* ptr);
    void SetStencil(uint32_t stencil);
    uint16_t SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void SetScissorCached(uint16_t id);
    void SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height);
    void SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height);
    //void SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth,
    //    uint8_t stencil);
    uint32_t Submit(uint8_t id, uint16_t pipeline, uint32_t depth);
    int Flush();

#ifdef ANT2D_DEBUG
    auto &GetSortKeys()
    {
        return sort_keys_;
    }
    auto &GetSortValues()
    {
        return sort_values_;
    }
    auto &GetDrawCallList()
    {
        return draw_call_list_;
    }

    auto &GetDrawCallNum()
    {
        return draw_call_num_;
    }

    auto &GetSortKey()
    {
        return sort_key_;
    }

    auto &GetDrawCall()
    {
        Info("get draw call {}", draw_call_.num_);
        return draw_call_;
    }

    auto GetUniformblockBegin()
    {
        return uniformblock_begin_;
    }

    auto GetUniformblockEnd()
    {
        return uniformblock_end_;
    }

#endif
};
} // namespace ant2d
#define SharedRenderQueue \
    ::ant2d::Singleton<::ant2d::RenderQueue>()

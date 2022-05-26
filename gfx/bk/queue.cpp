#include <gfx/bk/queue.h>
#include "ZipIterator.hpp"
#include <algorithm>

using namespace ant2d;

RenderQueue::RenderQueue(ResManager *R):ubb_(new UniformblockBuffer()),
        ctx_(new RenderContext(R, ubb_.get())), rm_(R)
{
}

void RenderQueue::Init()
{
}

void RenderQueue::Reset(uint16_t w, uint16_t h, float pr)
{
    ctx_->SetwRect(w, h);
    ctx_->SetPixelRatio(pr);
}

void RenderQueue::SetState(uint64_t state, uint32_t rgba)
{
    draw_call_.state_ = state;
}

void RenderQueue::SetIndexBuffer(uint16_t id, uint16_t first_index, uint16_t num)
{
    draw_call_.index_buffer_ = id;
    draw_call_.first_index_ = first_index;
    draw_call_.num_ = num;
}

void RenderQueue::SetVertexBuffer(uint8_t stream, uint16_t id)
{
    if (stream < 0 || stream >= 2) {
        //todo err handle
        return;
    }
    auto vbStream = &draw_call_.vertex_buffers_[stream];
    vbStream->vertex_buffer = id;
}

void RenderQueue::SetTexture(uint8_t stage, uint16_t tex_id)
{
    if (stage < 0 || stage >= 2) {
        //log.Printf("Not suppor texture location: %d", stage)
        //todo err handle
        return;
    }

    draw_call_.textures_[stage] = tex_id;
}

void RenderQueue::SetUniformblock(uint16_t id, uint8_t *ptr)
{
    auto ub = rm_->GetUniformblock(id);
    if (ub) {
        uint32_t opcode = Uniformblock::encode(ub->GetStage(), ub->GetSlot(), ub->GetSize());
        ubb_->WriteUInt32(opcode);
        ubb_->Copy(ptr, static_cast<uint32_t>(ub->GetSize()));
    }
}

void RenderQueue::SetStencil(uint32_t stencil)
{
    draw_call_.stencil_ = stencil;
}

uint16_t RenderQueue::SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t id = ctx_->AddClipRect(x, y, width, height);
    draw_call_.scissor_ = id;
    return id;
}

void RenderQueue::SetScissorCached(uint16_t id)
{
    draw_call_.scissor_ = id;
}

void RenderQueue::SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (id < 0 || id >= 4) {
        //log.Printf("Not support view id: %d", id)
        return;
    }
    scissors_[id] = Rect{x, y, width, height};
}

void RenderQueue::SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (id < 0 || id >= 4) {
        //log.Printf("Not support view id: %d", id)
        return;
    }
    viewports_[id] = Rect{x, y, width, height};
}


void RenderQueue::SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth, uint8_t stencil)
{
    if (id < 0 || id >= 4) {
        //log.Printf("Not support view id: %d", id)
        return;
    }

    auto clear = &clears_[id];
    clear->flags = flags;
    clear->rgba = rgba;
    clear->depth = depth;
    clear->stencil = stencil;
}

uint32_t RenderQueue::Submit(uint8_t id, uint16_t shader, uint64_t depth)
{
    uniformblock_end_ = static_cast<uint16_t>(ubb_->GetPos());
    sort_key_.layer_ = static_cast<uint16_t>(id);
    sort_key_.order_ = static_cast<uint16_t>(depth+0xFFFF>>1);

    sort_key_.shader_ = shader;
    sort_key_.blend_ = 0;
    sort_key_.texture_ = draw_call_.textures_[0];

    sort_keys_[draw_call_num_] = sort_key_.Encode();
    sort_values_[draw_call_num_] = draw_call_num_;

    draw_call_.uniformblock_begin_ = uniformblock_begin_;
    draw_call_.uniformblock_end_ = uniformblock_end_;

    draw_call_list_[draw_call_num_] = draw_call_;
    draw_call_num_++;

    draw_call_.reset();
    uniformblock_begin_ = static_cast<uint16_t>(ubb_->GetPos());
    return 0;
}

int RenderQueue::Flush()
{
    auto num = draw_call_num_;

    std::vector<uint64_t> sort_keys = {sort_keys_.begin(), sort_keys_.begin() + num};
    std::vector<uint16_t> sort_values = {sort_values_.begin(), sort_values_.begin() + num};
    std::vector<RenderDraw> draw_list = {draw_call_list_.begin(), draw_call_list_.begin() + num};

    auto kv_zip = Zip(sort_keys, sort_values);
    switch (sort_mode_) {
        case kAscending:
            std::sort(kv_zip.begin(), kv_zip.end(), [](const auto &x, const auto &y) {
                return std::get<0>(x) > std::get<0>(y);
            });
            break;
        case kDescending:
            std::sort(kv_zip.begin(), kv_zip.end(), [](const auto &x, const auto &y) {
                return std::get<0>(x) < std::get<0>(y);
            });
            break;
    }

    ctx_->Draw(std::move(sort_keys), std::move(sort_values), std::move(draw_list));

    draw_call_num_ = 0;
    uniformblock_begin_ = 0;
    uniformblock_end_ = 0;
    ubb_->Reset();
    ctx_->Reset();
    return 0;
}

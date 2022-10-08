#include "ZipIterator.hpp"
#include <algorithm>
#include <gfx/bk/queue.h>
#include <utils/debug.h>

using namespace ant2d;

RenderQueue::RenderQueue()
    : sort_mode_(SortMode::kAscending)
    , sort_keys_()
    , sort_values_()
    , draw_call_list_()
    , draw_call_num_(0)
    , sort_key_ {}
    , draw_call_ {}
    , uniformblock_begin_(0)
    , uniformblock_end_(0)
    , viewports_()
    , scissors_()
    , rm_(&SharedResManager)
    , uniformblock_buffer_(new UniformblockBuffer())
    , ctx_(new RenderContext(&SharedResManager, uniformblock_buffer_.get()))
{
}

void RenderQueue::Init()
{
    ctx_->Init();
}

void RenderQueue::Reset(uint16_t w, uint16_t h, float pr)
{
    ctx_->SetwRect(w, h);
    ctx_->SetPixelRatio(pr);
}

// void RenderQueue::SetState(uint64_t state, uint32_t rgba)
//{
//     draw_call_.state_ = state;
// }

void RenderQueue::SetIndexBuffer(uint16_t id, uint16_t first_index, uint16_t num)
{
    Info("set index buffer {}--{}--{}", id, first_index, num);
    draw_call_.index_buffer_ = ResManager::TripType(id);
    draw_call_.first_index_ = first_index;
    draw_call_.num_ = num;
}

void RenderQueue::SetVertexBuffer(uint8_t stream, uint16_t id)
{
    if (stream < 0 || stream >= 2) {
        Error("vertex buffer stream can only between 0~2, not {}", stream);
        return;
    }
    draw_call_.vertex_buffers_[stream] = ResManager::TripType(id);
}

void RenderQueue::SetTexture(uint8_t stage, uint16_t tex_id)
{
    if (stage < 0 || stage >= 2) {
        Error("texture location can only between 0~2, not {}", stage);
        return;
    }

    draw_call_.textures_[stage] = ResManager::TripType(tex_id);
}

void RenderQueue::SetUniformblock(uint16_t id, uint8_t* ptr)
{
    auto ub = rm_->GetUniformblock(id);
    if (ub) {
        Info("encode ub stage--{}, slot--{}, size--{}", ub->GetStage(), ub->GetSlot(), ub->GetSize());
        uint32_t opcode = Uniformblock::Encode(ub->GetStage(), ub->GetSlot(), ub->GetSize());
        uniformblock_buffer_->WriteUInt32(opcode);
        uniformblock_buffer_->Copy(ptr, static_cast<uint32_t>(ub->GetSize()));
    }
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
        Error("Not support view id {}", id);
        return;
    }
    scissors_[id] = Rect { x, y, width, height };
}

void RenderQueue::SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (id < 0 || id >= 4) {
        Error("Not support view id {}", id);
        return;
    }
    viewports_[id] = Rect { x, y, width, height };
}

/*
void RenderQueue::SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth, uint8_t stencil)
{
    if (id < 0 || id >= 4) {
        // log.Printf("Not support view id: %d", id)
        return;
    }

    auto clear = &clears_[id];
    clear->flags = flags;
    clear->rgba = rgba;
    clear->depth = depth;
    clear->stencil = stencil;
}
*/

uint32_t RenderQueue::Submit(uint8_t id, uint16_t pipeline, uint32_t depth)
{
    Info("RenderQueue submit {}--{}--{}--{}", draw_call_num_, id, pipeline, depth);
    uniformblock_end_ = static_cast<uint16_t>(uniformblock_buffer_->GetPos());
    sort_key_.layer_ = static_cast<uint16_t>(id);
    sort_key_.order_ = static_cast<uint16_t>(depth + (0xFFFF >> 1));

    sort_key_.pipeline_ = ResManager::TripType(pipeline);
    Info("befor encode sort_key_.pipeline_ {}", sort_key_.pipeline_);
    sort_key_.blend_ = 0;
    sort_key_.texture_ = draw_call_.textures_[0];

    sort_keys_[draw_call_num_] = sort_key_.Encode();
    sort_values_[draw_call_num_] = draw_call_num_;

    draw_call_.uniformblock_begin_ = uniformblock_begin_;
    draw_call_.uniformblock_end_ = uniformblock_end_;

    draw_call_list_[draw_call_num_] = draw_call_;
    draw_call_num_++;

    draw_call_.Reset();
    uniformblock_begin_ = static_cast<uint16_t>(uniformblock_buffer_->GetPos());
    return 0;
}

int RenderQueue::Flush()
{
    auto num = draw_call_num_;

    std::vector<uint64_t> sort_keys = { sort_keys_.begin(), sort_keys_.begin() + num };
    std::vector<uint16_t> sort_values = { sort_values_.begin(), sort_values_.begin() + num };
    std::vector<RenderDraw> draw_list = { draw_call_list_.begin(), draw_call_list_.begin() + num };

    auto kv_zip = Zip(sort_keys, sort_values);
    switch (sort_mode_) {
    case SortMode::kAscending:
        Info("key sort1");
        std::sort(kv_zip.begin(), kv_zip.end(), [](const auto& x, const auto& y) {
            return std::get<0>(x) < std::get<0>(y);
        });
        break;
    case SortMode::kDescending:
        Info("key sort2");
        std::sort(kv_zip.begin(), kv_zip.end(), [](const auto& x, const auto& y) {
            return std::get<0>(x) > std::get<0>(y);
        });
        break;
    case SortMode::kSequential:
        Info("key sort3");
        break;
    }

    Info("before draw {}", num);
    for (auto key : sort_keys) {
        Info("key item {}", key);
    }

    for (auto value : sort_values) {
        Info("value item {}", value);
    }

    for (auto& draw : draw_list) {
        Info("draw item {}", draw.index_buffer_);
    }
    ctx_->Draw(std::move(sort_keys), std::move(sort_values), std::move(draw_list));
    Info("after draw");
    draw_call_num_ = 0;
    uniformblock_begin_ = 0;
    uniformblock_end_ = 0;
    uniformblock_buffer_->Reset();
    ctx_->Reset();
    return 0;
}

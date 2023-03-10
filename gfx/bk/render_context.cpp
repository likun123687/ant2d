#include <gfx/bk/render_context.h>
#include "sokol_app.h"
#include "sokol_glue.h"
#include <utils/debug.h>

namespace ant2d {

RenderContext::RenderContext(ResManager* res_manager, UniformblockBuffer* uniformblock_buffer)
    : res_manager_(res_manager)
    , uniformblock_buffer_(uniformblock_buffer)
    , w_rect_ {}
    , pixel_ratio_ { 0.0f }
    , clips_ {}
    , bk_state_ {}
{
    clips_.resize(1);
}

void RenderContext::Reset()
{
    // clips_ = { clips_.begin(), clips_.begin() + 1 };
    clips_.erase(clips_.begin() + 1, clips_.end());
}

void RenderContext::Init()
{
    sg_desc desc = {};
    desc.context = sapp_sgcontext();
    sg_setup(&desc);
}

uint16_t RenderContext::AddClipRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t index = static_cast<uint16_t>(clips_.size());
    auto ratio = pixel_ratio_;
    clips_.push_back(Rect(
        static_cast<uint16_t>(static_cast<float>(x) * ratio),
        static_cast<uint16_t>(static_cast<float>(y) * ratio),
        static_cast<uint16_t>(static_cast<float>(w) * ratio),
        static_cast<uint16_t>(static_cast<float>(h) * ratio)));
    return index;
}

void RenderContext::UpdateScissor(RenderDraw& draw, RenderDraw& current_state)
{
    if (draw.scissor_ != current_state.scissor_) {
        current_state.scissor_ = draw.scissor_;
        auto clip = clips_[draw.scissor_];

        if (!clip.IsZero()) {
            sg_apply_viewport(clip.x_, clip.y_, clip.w_, clip.h_, false);
        }
    }
}

void RenderContext::UpdateUniformblockBind(RenderDraw& draw, RenderDraw& current_state)
{
    if (draw.uniformblock_begin_ < draw.uniformblock_end_) {
        BindUniformblock(static_cast<uint32_t>(draw.uniformblock_begin_), static_cast<uint32_t>(draw.uniformblock_end_));
    }
}

void RenderContext::UpdateTextureBind(RenderDraw& draw, RenderDraw& current_state)
{
    for (int stage = 0; stage < 2; stage++) {
        auto bind = draw.textures_[stage];
        auto current = current_state.textures_[stage];
        if (kInvalidId != bind && current != bind) {
            auto texture = res_manager_->GetTexture(bind, true);
            bk_state_.bind.fs_images[stage] = texture->GetId();
            current_state.textures_[stage] = bind;
        }
    }
}

void RenderContext::UpdatePipeline(uint16_t& old_pipeline, uint16_t& new_pipeline)
{
    if (new_pipeline != kInvalidId && old_pipeline != new_pipeline) {
        auto sg_p_id = res_manager_->GetPipeline(new_pipeline, true)->GetPipeLineId();
        sg_apply_pipeline(sg_p_id);
        old_pipeline = new_pipeline;
    }
}

void RenderContext::UpdateBufferBind(RenderDraw& draw, RenderDraw& current_state)
{
    auto ib = draw.index_buffer_;
    if (ib != kInvalidId && ib != current_state.index_buffer_) {
        bk_state_.bind.index_buffer = res_manager_->GetIndexBuffer(ib, true)->GetId();
        current_state.index_buffer_ = ib;
    }

    for (int stage = 0; stage < 2; stage++) {
        auto vb = draw.vertex_buffers_[stage];
        if (vb.vertex_id != kInvalidId && vb.vertex_id != current_state.vertex_buffers_[stage].vertex_id) {
            bk_state_.bind.vertex_buffers[stage] = res_manager_->GetVertexBuffer(vb.vertex_id, true)->GetId();
            current_state.vertex_buffers_[stage].vertex_id = vb.vertex_id;
        }

        if (vb.offset != current_state.vertex_buffers_[stage].offset) {
            bk_state_.bind.vertex_buffer_offsets[stage] = vb.offset;
            current_state.vertex_buffers_[stage].offset = vb.offset;
        }
    }
}

void RenderContext::DoDraw(RenderDraw& draw, RenderDraw& current_state)
{
    //Info("bk_state bind {}--{}--{}", bk_state_.bind.index_buffer.id, bk_state_.bind.vertex_buffers[0].id, bk_state_.bind.fs_images[0].id);
    // TODO ????????????
    if (bk_state_.bind.index_buffer.id == SG_INVALID_ID || bk_state_.bind.vertex_buffers[0].id == SG_INVALID_ID || bk_state_.bind.fs_images[0].id == SG_INVALID_ID) {
        return;
    }

    sg_apply_bindings(&bk_state_.bind);

    if (draw.index_buffer_ != kInvalidId) {
        auto offset = int(draw.first_index_) * 1; // 2 = sizeOf(unsigned_short)
        sg_draw(offset, draw.num_, 1);
    } else {
        sg_draw(draw.first_index_, draw.num_, 1);
    }
}

void RenderContext::Draw(std::vector<uint64_t>& sort_keys, std::vector<uint16_t>& sort_values,
    std::vector<RenderDraw>& draw_list)
{
    auto current_state = RenderDraw {};
    auto pipeline = kInvalidId;
    auto sort_key = SortKey {};
    // uint8_t prim_index = static_cast<uint8_t>(static_cast<uint64_t>(0) >> kState.pt_shift);
    //  uint64_t state_bits = State.kDepthWrite | ST.DEPTH_TEST_MASK | ST.RGB_WRITE | ST.ALPHA_WRITE | ST.BLEND_MASK | ST.PT_MASK;
    for (int i = 0; i < sort_keys.size(); i++) {
        auto encoded_key = sort_keys[i];
        auto item_id = sort_values[i];
        sort_key.Decode(encoded_key);

        auto draw = draw_list[item_id];

        UpdatePipeline(pipeline, sort_key.pipeline_);

        // scissor
        UpdateScissor(draw, current_state);

        // 6. uniform binding
        UpdateUniformblockBind(draw, current_state);

        // 7 .texture bind
        UpdateTextureBind(draw, current_state);

        // 8. index & vertex binding TODO ?????? attribute ??????
        UpdateBufferBind(draw, current_state);

        // 9. draw
        DoDraw(draw, current_state);
    }
}

void RenderContext::BindUniformblock(uint32_t begin, uint32_t end)
{
    uniformblock_buffer_->Seek(begin);

    while (uniformblock_buffer_->GetPos() < end) {
        auto opcode = uniformblock_buffer_->ReadUInt32();

        uint8_t stage, slot, size;
        Uniformblock::Decode(opcode, &stage, &slot, &size);
        auto data = uniformblock_buffer_->ReadPointer(size);
        auto sg_data = sg_range { data, size };

        sg_apply_uniforms(static_cast<sg_shader_stage>(stage), static_cast<int>(slot),
            &sg_data);
    }
}

void RenderContext::SetwRect(uint16_t w, uint16_t h)
{
    w_rect_.w_ = w;
    w_rect_.h_ = h;
}

void RenderContext::SetPixelRatio(float pixel_ratio)
{
    pixel_ratio_ = pixel_ratio;
}
} // namespace ant2d

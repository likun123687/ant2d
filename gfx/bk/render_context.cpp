#include <gfx/bk/render_context.h>
#include <gfx/bk/global_state.h>

using namespace ant2d;

RenderContext::RenderContext(ResManager* res_manager, UniformblockBuffer* uniformblock_buffer)
    : res_manager_(res_manager)
    , uniformblock_buffer_(uniformblock_buffer)
{
    clips_.reserve(1);
}

void RenderContext::Reset()
{
    clips_ = { clips_.begin(), clips_.begin() + 1 };
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

void RenderContext::UpdateStencil(RenderDraw& draw, RenderDraw& current_state)
{
    auto new_stencil = draw.stencil_;
    auto changed_stencil = current_state.stencil_ ^ draw.stencil_;
    current_state.stencil_ = new_stencil;

    if (changed_stencil != 0) {
        if (new_stencil != 0) {
            pipeline_desc_.stencil.enabled = true;
        } else {
            pipeline_desc_.stencil.enabled = false;
        }
    }
}

void RenderContext::UpdateStateBind(RenderDraw& draw, RenderDraw& current_state)
{
    uint64_t state_bits = kState.depth_write | kState.depth_test_mask | kState.rgb_write | kState.alpha_write | kState.blend_mask | kState.pt_mask;

    auto new_flags = draw.state_;
    auto changed_flags = current_state.state_ ^ draw.state_;
    current_state.state_ = new_flags;

    if ((state_bits & changed_flags) != 0) {
        BindState(changed_flags, new_flags);
        auto pt = new_flags & kState.pt_mask;
        auto prim_index = static_cast<uint8_t>(pt >> kState.pt_shift);
        // auto prim = g_PrimInfo[prim_index];
        pipeline_desc_.primitive_type = kPrimInfo[prim_index];
    }
}

void RenderContext::UpdateProgram(RenderDraw& draw, RenderDraw& current_state, SortKey& sort_key, uint16_t& shader_id, bool& program_changed)
{
    if (sort_key.shader_ != shader_id) {
        shader_id = sort_key.shader_;
        pipeline_desc_.shader = res_manager_->GetShader(shader_id)->GetShdId();
        program_changed = true;
    }
}

void RenderContext::UpdateUniformblockBind(RenderDraw& draw, RenderDraw& current_state)
{
    if (draw.uniformblock_begin_ < draw.uniformblock_end_) {
        BindUniformblock(static_cast<uint32_t>(draw.uniformblock_begin_), static_cast<uint32_t>(draw.uniformblock_end_));
    }
}

void RenderContext::UpdateTextureBind(RenderDraw& draw, RenderDraw& current_state, bool program_changed)
{
    for (int stage = 0; stage < 2; stage++) {
        auto bind = draw.textures_[stage];
        auto current = current_state.textures_[stage];
        if (kInvalidId != bind) {
            if (current != bind || program_changed) {
                auto texture = res_manager_->GetTexture(bind);
                app_state_.bind.fs_images[stage] = texture->GetId();
            }
        }
        current_state.textures_[stage] = bind;
    }
}

void RenderContext::UpdateBufferBind(RenderDraw& draw, RenderDraw& current_state, uint16_t shader_id)
{
    auto shader = res_manager_->GetShader(shader_id);
    BindAttributes(shader, &draw);

    auto ib = draw.index_buffer_;
    if (ib != kInvalidId && ib != current_state.index_buffer_) {
        app_state_.bind.index_buffer = res_manager_->GetIndexBuffer(ib)->GetId();
        current_state.index_buffer_ = ib;
    }
}

void RenderContext::DoDraw(RenderDraw& draw, RenderDraw& current_state)
{
    pipeline_desc_.index_type = SG_INDEXTYPE_UINT16;
    if (draw.index_buffer_ != kInvalidId) {
        auto offset = int(draw.first_index_) * 2; // 2 = sizeOf(unsigned_short)
        sg_draw(offset, draw.num_, 1);
    } else {
        sg_draw(draw.first_index_, draw.num_, 1);
    }
}

void RenderContext::Draw(std::vector<uint64_t> sort_keys, std::vector<uint16_t> sort_values,
    std::vector<RenderDraw> draw_list)
{
    auto current_state = RenderDraw {};
    auto shader_id = kInvalidId;
    auto sort_key = SortKey {};
    //uint8_t prim_index = static_cast<uint8_t>(static_cast<uint64_t>(0) >> kState.pt_shift);
    // uint64_t state_bits = State.kDepthWrite | ST.DEPTH_TEST_MASK | ST.RGB_WRITE | ST.ALPHA_WRITE | ST.BLEND_MASK | ST.PT_MASK;

    for (int i = 0; i < sort_keys.size(); i++) {
        auto encoded_key = sort_keys[i];
        auto item_id = sort_values[i];
        sort_key.Decode(encoded_key);

        auto draw = draw_list[item_id];

        // scissor
        UpdateScissor(draw, current_state);

        // stencil
        UpdateStencil(draw, current_state);

        // 4. state binding
        UpdateStateBind(draw, current_state);

        // 5. Update program
        bool program_changed;
        UpdateProgram(draw, current_state, sort_key, shader_id, program_changed);

        // 6. uniform binding
        UpdateUniformblockBind(draw, current_state);

        // 7 .texture bind
        UpdateTextureBind(draw, current_state, program_changed);

        // 8. index & vertex binding TODO 优化 attribute 绑定
        UpdateBufferBind(draw, current_state, shader_id);

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

void RenderContext::BindState(uint64_t changed_flags, uint64_t new_flags)
{
    if ((changed_flags & kState.depth_write) != 0) {
        pipeline_desc_.depth.write_enabled = ((new_flags & kState.depth_write) != 0);
        // gl.DepthMask(new_flags&ST.DEPTH_WRITE != 0)
    }

    if ((changed_flags & (kState.alpha_write | kState.rgb_write)) != 0) {
        auto alpha = (new_flags & kState.alpha_write) != 0;
        auto rgb = (new_flags & kState.rgb_write) != 0;
        sg_color_mask color_mask = SG_COLORMASK_RGBA;
        if (alpha && rgb) {
            color_mask = SG_COLORMASK_RGBA;
        } else if (alpha && !rgb) {
            color_mask = SG_COLORMASK_A;
        } else if (!alpha && rgb) {
            color_mask = SG_COLORMASK_RGB;
        } else if (!alpha && !rgb) {
            color_mask = SG_COLORMASK_NONE;
        }

        pipeline_desc_.colors[0].write_mask = color_mask;
    }

    if ((changed_flags & kState.depth_test_mask) != 0) {
        auto _func = (new_flags & kState.depth_test_mask) >> kState.depth_test_shift;

        if (_func != 0) {
            pipeline_desc_.depth.compare = kCmpFunc[_func];
        } else {
            if ((new_flags & kState.depth_write) != 0) {
                pipeline_desc_.depth.compare = SG_COMPAREFUNC_ALWAYS;
            }
        }
    }

    /// 所谓 blend independent 可以实现顺序无关的 alpha 混合
    /// http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/
    if ((changed_flags & kState.blend_mask) != 0) {
        auto blend = static_cast<uint16_t>(new_flags & kState.blend_mask) >> kState.blend_shift;
        if (blend != 0) {
            // gl.Enable(gl.BLEND)
            // gl.BlendFunc(g_Blend[blend].Src, g_Blend[blend].Dst)

            pipeline_desc_.colors[0].blend.enabled = true;
            pipeline_desc_.colors[0].blend.src_factor_rgb = kBlend[blend].src;
            pipeline_desc_.colors[0].blend.src_factor_alpha = kBlend[blend].src;

            pipeline_desc_.colors[0].blend.dst_factor_rgb = kBlend[blend].dst;
            pipeline_desc_.colors[0].blend.dst_factor_alpha = kBlend[blend].dst;

        } else {
            pipeline_desc_.colors[0].blend.enabled = false;
        }
    }
}

void RenderContext::BindAttributes(Shader* sh, RenderDraw* draw)
{
    uint16_t bind_stream = kUInt16Max;
    //uint16_t bind_stride = 0;
    auto streams = draw->vertex_buffers_;

    uint16_t numAttr = sh->GetNumAttr();

    for (int i = 0; i < numAttr; i++) {
        auto bind = sh->GetAttribBind(i);
        auto stream = streams[bind.stream_];

        if (bind.stream_ != bind_stream) {
            auto buffer = res_manager_->GetVertexBuffer(stream.vertex_buffer);
            app_state_.bind.vertex_buffers[0] = buffer->GetId();

            bind_stream = bind.stream_;
        }

        pipeline_desc_.layout.attrs[bind.slot_].format = bind.vertex_format_;
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

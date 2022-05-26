#include <gfx/bk/render_context.h>
#include <gfx/bk/sort_key.h>

using namespace ant2d;

RenderContext::RenderContext(ResManager *res_manager, UniformblockBuffer *ubb):res_manager_(res_manager),ubb_(ubb)
{
    clips_.reserve(1);
}

void RenderContext::Reset()
{
    clips_ = {clips_.begin(), clips_.begin() + 1};
}

uint16_t RenderContext::AddClipRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t index = static_cast<uint16_t>(clips_.size());
    auto ratio = pixel_ratio_;
    clips_.push_back(Rect(
                static_cast<uint16_t>(static_cast<float>(x) * ratio),
                static_cast<uint16_t>(static_cast<float>(y) * ratio),
                static_cast<uint16_t>(static_cast<float>(w) * ratio),
                static_cast<uint16_t>(static_cast<float>(h) * ratio)
            ));
    return index;
}

void RenderContext::Draw(std::vector<uint64_t> sort_keys, std::vector<uint16_t> sort_values,
        std::vector<RenderDraw> draw_list)
{
    auto current_state = RenderDraw{};
    auto shader_id = kInvalidId;
    auto sort_key = SortKey{};
    uint8_t prim_index = static_cast<uint8_t>(static_cast<uint64_t>(0) >> kState.pt_shift);
    //uint64_t state_bits = State.kDepthWrite | ST.DEPTH_TEST_MASK | ST.RGB_WRITE | ST.ALPHA_WRITE | ST.BLEND_MASK | ST.PT_MASK;
    uint64_t state_bits = kState.depth_write | kState.depth_test_mask | kState.rgb_write | kState.alpha_write | kState.blend_mask | kState.pt_mask;

    for (int i = 0; i < sort_keys.size(); i++) {
        auto encoded_key = sort_keys[i];
        auto item_id = sort_values[i];
        sort_key.Decode(encoded_key);

        auto draw = draw_list[item_id];

        auto newFlags = draw.state_;
        auto changedFlags = current_state.state_ ^ draw.state_;
        current_state.state_ = newFlags;

        auto newStencil = draw.stencil_;
        auto changedStencil = current_state.stencil_ ^ draw.stencil_;
        current_state.stencil_ = newStencil;

        //scissor
        if (draw.scissor_ != current_state.scissor_) {
            current_state.scissor_ = draw.scissor_;
            auto clip = clips_[draw.scissor_];

            if (!clip.isZero()) {
                sg_apply_viewport(clip.x_, clip.y_, clip.w_, clip.h_, false);
            }
        }

        //stencil
        if (changedStencil != 0) {
            if (newStencil != 0) {
                pipeline_desc_.stencil.enabled = true;
            } else {
                pipeline_desc_.stencil.enabled = false;
            }
        }

        // 4. state binding
        if ((state_bits & changedFlags) != 0) {
            BindState(changedFlags, newFlags);
            auto pt = newFlags & kState.pt_mask;
            auto prim_index = static_cast<uint8_t>(pt >> kState.pt_shift);
            //auto prim = g_PrimInfo[prim_index];
            pipeline_desc_.primitive_type = kPrimInfo[prim_index];
        }

        bool programChanged;
        // 5. Update program
        if (sort_key.shader_ != shader_id) {
            shader_id = sort_key.shader_;
            pipeline_desc_.shader = res_manager_->GetShader(shader_id)->GetShdId();
            programChanged = true;
        }

        // 6. uniform binding
        if (draw.uniformblock_begin_ < draw.uniformblock_end_) {
            BindUniformblock(static_cast<uint32_t>(draw.uniformblock_begin_), static_cast<uint32_t>(draw.uniformblock_end_));
        }

        //7 .texture bind
        for (int stage = 0; stage < 2; stage++){
            auto bind = draw.textures_[stage];
            auto current = current_state.textures_[stage];
            if (kInvalidId != bind) {
                if (current != bind || programChanged) {
                    auto texture = res_manager_->GetTexture(bind);
                    app_state_.bind.fs_images[stage] = texture->GetId();
                }
            }
            current_state.textures_[stage] = bind;
        }

        // 8. index & vertex binding TODO 优化 attribute 绑定
        //auto shader = res_manager_.shaders[shader_id];
        auto shader = res_manager_->GetShader(shader_id);
        BindAttributes(shader, &draw);

        auto ib = draw.index_buffer_;
        if (ib != kInvalidId && ib != current_state.index_buffer_) {
            app_state_.bind.index_buffer = res_manager_->GetIndexBuffer(ib)->GetId();
            current_state.index_buffer_ = ib;
        }

        // 9. draw
        pipeline_desc_.index_type = SG_INDEXTYPE_UINT16;
        if (draw.index_buffer_ != kInvalidId) {
            auto offset = int(draw.first_index_) * 2; // 2 = sizeOf(unsigned_short)
            sg_draw(offset, draw.num_, 1);
        } else {
            sg_draw(draw.first_index_, draw.num_, 1);
        }
    }
}

void RenderContext::BindUniformblock(uint32_t begin, uint32_t end) {
    ubb_->Seek(begin);

    while(ubb_->GetPos() < end) {
        auto opcode = ubb_->ReadUInt32();

        uint8_t stage, slot, size;
        Uniformblock::decode(opcode, &stage, &slot, &size);
        auto data = ubb_->ReadPointer(size);

        sg_apply_uniforms(static_cast<sg_shader_stage>(stage), static_cast<int>(slot), 
                &sg_range{data, size});
    }
}

void RenderContext::BindState(uint64_t changed_flags, uint64_t new_flags)
{
    if (changed_flags&kState.depth_write != 0) {
        pipeline_desc_.depth.write_enabled = (new_flags&kState.depth_write != 0);
        //gl.DepthMask(new_flags&ST.DEPTH_WRITE != 0)
    }

    if (changed_flags&(kState.alpha_write|kState.rgb_write) != 0) {
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

    if (changed_flags&kState.depth_test_mask != 0) {
        auto _func = (new_flags & kState.depth_test_mask) >> kState.depth_test_shift;

        if (_func != 0) {
            pipeline_desc_.depth.compare = kCmpFunc[_func];
        } else {
            if (new_flags&kState.depth_write != 0) {
                pipeline_desc_.depth.compare = SG_COMPAREFUNC_ALWAYS;
            }
        }
    }

    /// 所谓 blend independent 可以实现顺序无关的 alpha 混合
    /// http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/
    if (changed_flags&kState.blend_mask != 0) {
        auto blend = static_cast<uint16_t>(new_flags&kState.blend_mask) >> kState.blend_shift;
        if (blend != 0) {
            //gl.Enable(gl.BLEND)
            //gl.BlendFunc(g_Blend[blend].Src, g_Blend[blend].Dst)

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

void RenderContext::BindAttributes(Shader *sh, RenderDraw *draw)
{
    uint16_t bindStream = kUInt16Max;
    uint16_t bindStride = 0;
    auto streams = draw->vertex_buffers_;

    uint16_t numAttr = sh->GetNumAttr();

    for (int i = 0; i < numAttr; i++) {
        auto bind = sh->GetAttribBind(i);
        auto stream = streams[bind.stream_];

        if (bind.stream_ != bindStream) {
            auto buffer = res_manager_->GetVertexBuffer(stream.vertex_buffer);
            app_state_.bind.vertex_buffers[0] = buffer->GetId();

            bindStream = bind.stream_;
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

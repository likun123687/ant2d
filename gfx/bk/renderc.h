class RenderContext
{
    private:
        const ResManager *R_;
        const UniformBuffer *ub_;
        Rect wRect_;
        float pixelRatio_;
        std::vector<Rect> clips_;

    public:
        RenderContext(ResManager *R, UniformBuffer *ub):R_(R),ub_(ub)
        {
            clips_.reserve(1);
        }

        void Reset()
        {
            clips_ = {clips_.begin(), clips_.begin() + 1};
        }

        uint16_t AddClipRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
        {
            uint16_t index = static_cast<uint16_t>(clips_.size());
            auto radio = pixelRatio_;
            clips_.push_back(Rect(
                        static_cast<uint16_t>(static_cast<float>(x) * ratio),
                        static_cast<uint16_t>(static_cast<float>(y) * ratio),
                        static_cast<uint16_t>(static_cast<float>(w) * ratio),
                        static_cast<uint16_t>(static_cast<float>(h) * ratio),
                    ));
            return index;
        }

        void Draw(std::vector<uint64_t> sortKeys, std::vector<uint16_t> sortValues, 
                std::vector<RenderDraw> drawList)
        {
            auto currentState = RenderDraw{};
            auto shaderId = InvalidId;
            auto key = SortKey{};
            auto primIndex = static_cast<uint8_t>(static_cast<uint64_t>(0) >> ST.PT_SHIFT);
            auto stateBits = ST.DEPTH_WRITE | ST.DEPTH_TEST_MASK | ST.RGB_WRITE | ST.ALPHA_WRITE | ST.BLEND_MASK | ST.PT_MASK;

            for (int i = 0; i < sortKeys.size(); i++){
                auto encodedKey = sortKeys[i];
                auto itemId = sortValues[i];
                key.Decode(encodedKey);

                auto draw = drawList[itemId];

                auto newFlags = draw.state;
                auto changedFlags = currentState.state ^ draw.state;
                currentState.state = newFlags;

                auto newStencil = draw.stencil;
                auto changedStencil = currentState.stencil ^ draw.stencil;
                currentState.stencil = newStencil;

                //scissor
                if (draw.scissor != currentState.scissor) {
                    currentState.scissor = draw.scissor;
                    auto clip = ctx_.clips[scissor];

                    if (clip.isZero()) {
                    } else {
                    }
                }

                //stencil
                if (changedStencil != 0) {
                    if (newStencil != 0) {
                    } else {
                    }
                }

                // 4. state binding
                if ((stateBits & changedFlags) != 0) {
                    ctx.bindState(changedFlags, newFlags);
                    auto pt = newFlags & ST.PT_MASK;
                    auto primIndex = uint8(pt >> ST.PT_SHIFT);
                    auto prim = g_PrimInfo[primIndex];
                }

                bool programChanged;

                // 5. Update program
                if (key.Shader != shaderId) {
                    shaderId = key.Shader;
                    auto id = ctx.R.shaders[shaderId].GLShader.Program;
                    gl.UseProgram(id);
                    programChanged = true;
                }

                // 6. uniform binding
                if (draw.uniformBegin < draw.uniformEnd) {
                    bindUniform(uint32(draw.uniformBegin), uint32(draw.uniformEnd));
                }

                for (int stage = 0; stage < 2; stage++){
                    auto bind = draw.textures[stage];
                    auto current = currentState.textures[stage];
                    if (InvalidId != bind) {
                        if (current != bind || programChanged) {
                            auto texture = R_.textures[bind];
                            texture.Bind(int32(stage));
                        }
                    }
                    currentState.textures[stage] = bind;
                }

                // 8. index & vertex binding TODO 优化 attribute 绑定
                auto shader = R_.shaders[shaderId];
                shader.BindAttributes(R_, draw.vertexBuffers[:]);

                auto ib = draw.indexBuffer;
                if (ib != InvalidId && != currentState.indexBuffer) {
                    gl.BindBuffer(gl.ELEMENT_ARRAY_BUFFER, ctx.R.indexBuffers[ib].Id);
                    currentState.indexBuffer = ib;
                }

                // 9. draw
                if (draw.indexBuffer != InvalidId) {
                    auto offset = int(draw.firstIndex) * 2 // 2 = sizeOf(unsigned_short)
                    gl.DrawElements(prim, int32(draw.num), gl.UNSIGNED_SHORT, offset)
                } else {
                    gl.DrawArrays(prim, int32(draw.firstIndex), int32(draw.num))
                    log.Println("gl.DrawArrays", prim, int32(draw.firstIndex), int32(draw.num))
                }

            }
        }

        void bindUniform(uint32_t begin, uint32_t end) {
            ub_.Seek(begin);

            for (auto ub = ctx.ub;;ub.GetPos() < end) {
                auto opcode = ub.ReadUInt32();

                if (opcode == uint32(UniformEnd)) {
                    break;
                }

                uint8_t  uType, loc, size, num;
                Uniform_decode(opcode, &uType, &loc, &size, &num);
                auto data = ub.ReadPointer(uint32(size) * uint32(num));

                switch (UniformType(uType)) {
                case UniformInt1:
                    gl.Uniform1iv(int32(loc), int32(num), (*int32)(data))
                case UniformVec1:
                    gl.Uniform1fv(int32(loc), int32(num), (*float32)(data))
                case UniformVec4:
                    gl.Uniform4fv(int32(loc), int32(num), (*float32)(data))
                case UniformMat3:
                    gl.UniformMatrix3fv(int32(loc), int32(num), false, (*float32)(data))
                case UniformMat4:
                    gl.UniformMatrix4fv(int32(loc), int32(num), false, (*float32)(data))
                case UniformSampler:
                    gl.Uniform1i(int32(loc), *(*int32)(data))
                }
            }
        }

        void bindState(uint64_t changedFlags, uint64_t newFlags) {
            if (changedFlags&ST.DEPTH_WRITE != 0) {
                gl.DepthMask(newFlags&ST.DEPTH_WRITE != 0)
                log.Printf("depth mask state: %v", ST.DEPTH_WRITE&newFlags != 0)
            }

            if (changedFlags&(ST.ALPHA_WRITE|ST.RGB_WRITE) != 0) {
                auto alpha = (newFlags & ST.ALPHA_WRITE) != 0;
                auto rgb = (newFlags & ST.RGB_WRITE) != 0;
                gl.ColorMask(rgb, rgb, rgb, alpha)

                log.Printf("color mask state: (%t, %t)", rgb, alpha)
            }

            if (changedFlags&ST.DEPTH_TEST_MASK != 0) {
                auto _func = (newFlags & ST.DEPTH_TEST_MASK) >> ST.DEPTH_TEST_SHIFT;

                if (_func != 0) {
                    gl.Enable(gl.DEPTH_TEST)
                    gl.DepthFunc(g_CmpFunc[_func])

                    log.Printf("set depth-test func: %d", _func)
                } else {
                    if (newFlags&ST.DEPTH_WRITE != 0) {
                        gl.Enable(gl.DEPTH_TEST)
                        gl.DepthFunc(gl.ALWAYS)

                        log.Println("set depth-test always")
                    } else {
                        gl.Disable(gl.DEPTH_TEST)

                        log.Println("disable depth-test")
                    }
                }
            }

            /// 所谓 blend independent 可以实现顺序无关的 alpha 混合
            /// http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/
            if (changedFlags&ST.BLEND_MASK != 0) {
                auto blend = uint16(newFlags&ST.BLEND_MASK) >> ST.BLEND_SHIFT;
                if (blend != 0) {
                    gl.Enable(gl.BLEND)
                    gl.BlendFunc(g_Blend[blend].Src, g_Blend[blend].Dst)
                } else {
                    gl.Disable(gl.BLEND)
                }
            }
        }
}

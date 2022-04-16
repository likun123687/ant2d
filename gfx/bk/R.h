namespace bk {
    const uint16_t InvalidId = 0x0000;
    const uint16_t UInt16Max = 0xFFFF;

    const uint16_t IdMask = 0x0FFF;
    const uint16_t IdTypeShift = 12;

    struct Memory {
        void *Data;
        uint32_t Size;
    }

    enum IdType {
        IdTypeIndex = 0,
        IdTypeVertex,
        IdTypeTexture,
        IdTypeLayout,
        IdTypeUniform,
        IdTypeShader,
    };

    const uint16_t MaxIndex   = 2 << 10;
    const uint16_t MaxVertex  = 2 << 10;
    const uint16_t MaxTexture = 1 << 10;
    const uint16_t MaxUniform = 32 * 8;
    const uint16_t MaxShader  = 32;

    class FreeList
    {
        public:
            uint16_t Pop()
            {
                uint16_t slot = 0;
                if (!slots_.empty()) {
                    slot = slots_.back();
                    slots_.pop_back();
                }
                return slot;
            }

            void Push(uint16_t slot)
            {
                slots_.push_back(slot);
            }

        private:
            std::vector<uint16_t> slots_;
    }

    class ResManager
    {
        private:
            std::array<IndexBuffer, MaxIndex> indexBuffers_;
            std::array<VertexBuffer, MaxVertex> vertexBuffers_;
            std::array<Texture2D, MaxTexture> textures;

            std::array<Uniform, MaxUniform> uniforms_;
            std::array<Shader, MaxShader> shaders_;

            uint16_t ibIndex_;
            uint16_t vbIndex_;
            uint16_t ttIndex_;
            uint16_t umIndex_;
            uint16_t shIndex_;

            // free list
            FreeList ibFrees_;
            FreeList vbFrees_;
            FreeList ttFrees_;
            FreeList umFrees_;
            FreeList shFrees_;

        public:
            ResManager()
            {
                ibIndex_ = 1;
                vbIndex_ = 1;
                ttIndex_ = 1;
                umIndex_ = 1;
                shIndex_ = 1;
            }

            std::tuple<uint16_t, IndexBuffer*> AllocIndexBuffer(Memory mem)
            {
                uint16_t id = 0;
                IndexBuffer *ib = nullptr;

                auto index = ibFrees_.Pop();
                if (index) {
                    id = index;
                    ib = &indexBuffers_[index];
                } else {
                    id = ibIndex_;
                    ib = indexBuffers_[ibIndex_];
                    ibIndex_++;
                }

                id = id | (IdTypeIndex << IdTypeShift);
                ib.Create(mem.Size, mem.Data, 0);

                return tuple(id, ib);
            }

            std::tuple<uint16_t, VertexBuffer*>AllocVertexBuffer(Memory mem, uint16_t stride)
            {
                uint16_t id = 0;
                VertexBuffer *vb = nullptr;

                auto index = vbFrees_.Pop();
                if (index) {
                    id = index;
                    vb = &vertexBuffers_[index];
                } else {
                    id = vbIndex_;
                    vb = vertexBuffers_[vbIndex_];
                    vbIndex_++;
                }

                id = id | (IdTypeIndex << IdTypeShift);
                vb.Create(mem.Size, mem.Data, 0);

                return tuple(id, vb);
            }

            std::tuple<uint16_t, Uniform*> AllocUniform(uint16_t shId, string name,
                     UniformType xType, uint32_t num) {
                uint16_t id = 0;
                Uniform *um = nullptr;

                auto index = umFrees_.Pop();
                if (index) {
                    id = index;
                    um = &uniforms_[index];
                } else {
                    id = umIndex_;
                    um = uniforms_[umIndex_];
                    umIndex_++;
                }
                id = id | (IdTypeIndex << IdTypeShift);

                auto sh = Shader(shId);
                um.create(sh.Program, name, xType, num);
                return tuple(id, vb);
            }

            std::tuple<uint16_t, Texture2D*> AllocTexture(ImageData *data)
            {
                uint16_t id = 0;
                Texture2D *tex = nullptr;

                auto index = ttFrees_.Pop();
                if (index) {
                    id = index;
                    tex = &textures_[index];
                } else {
                    id = ttIndex_;
                    tex = textures_[ttIndex_];
                    ttIndex_++;
                }
                id = id | (IdTypeIndex << IdTypeShift);
                tex.Create(img);
                return tuple(id, tex);
            }

            void Free(uint16_t id)
            {
                auto t = (id >> IdTypeShift) & 0x000F;
                auto v = id & IdMask;

                switch t {
                    case IdTypeIndex:
                        indexBuffers_[v].Destroy();
                        ibFrees_.Push(v);
                        break;
                    case IdTypeVertex:
                        vertexBuffers_[v].Destroy();
                        vbFrees_.Push(v)
                        break;
                    case IdTypeTexture:
                        textures_[v].Destroy();
                        ttFrees_.Push(v);
                        break;
                    case IdTypeLayout:
                        // todo
                    case IdTypeUniform:
                        umFrees_.Push(v);
                        break;
                    case IdTypeShader:
                        shaders_[v].Destroy();
                        shFrees_.Push(v);
                        break;
                }
            }

}

////// STATE MASK AND VALUE DEFINES
struct {
    uint64_t RGB_WRITE;
    uint64_t ALPHA_WRITE;
    uint64_t DEPTH_WRITE;

    uint64_t DEPTH_TEST_MASK;
    uint64_t DEPTH_TEST_SHIFT;

    uint64_t BLEND_MASK;
    uint64_t BLEND_SHIFT;

    uint64_t PT_MASK;
    uint64_t PT_SHIFT;
} ST = {
    0x0000000000000001,
    0x0000000000000002,
    0x0000000000000004,

    0x00000000000000F0,
    4,

    0x0000000000000F00,
    8,

    0x000000000000F000,
    12,
};

// zero means no depth-test
struct {
    uint64_t LESS;
    uint64_t LEQUAL;
    uint64_t EQUAL;

    uint64_t GEQUAL;
    uint64_t GREATER;
    uint64_t NOTEQUAL;

    uint64_t NEVER;
    uint64_t ALWAYS;
} ST_DEPTH = {
    0x0000000000000010,
    0x0000000000000020,
    0x0000000000000030,

    0x0000000000000040,
    0x0000000000000050,
    0x0000000000000060,

    0x0000000000000070,
    0x0000000000000080,
};

uint32_t g_CmpFunc[] = {
    0, // ignored
    gl.LESS,
    gl.LEQUAL,
    gl.EQUAL,
    gl.GEQUAL,
    gl.GREATER,
    gl.NOTEQUAL,
    gl.NEVER,
    gl.ALWAYS,
};

// zero means no blend
struct {
    uint64_t DEFAULT;
    uint64_t ISABLE;
    uint64_t ALPHA_PREMULTIPLIED;
    uint64_t ALPHA_NON_PREMULTIPLIED;
    uint64_t ADDITIVE;
} ST_BLEND = {
    0x0,
    0x0000000000000100,
    0x0000000000000200,
    0x0000000000000300,
    0x0000000000000400,
}

struct {
    uint32_t Src;
    uint32_t Dst;
} g_Blend[] = {
    {0, 0},
    {gl.ONE, gl.ZERO},
    {gl.ONE, gl.ONE_MINUS_SRC_ALPHA},
    {gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA},
    {gl.SRC_ALPHA, gl.ONE},
}

struct {
   uint64_t TRIANGLES;
   uint64_t TRIANGLE_STRIP;
   uint64_t LINES;
   uint64_t LINE_STRIP;
   uint64_t POINTS;
} ST_PT = {
    0x0000000000000000,
    0x0000000000001000,
    0x0000000000002000,
    0x0000000000003000,
    0x0000000000004000,
}

uint32_t g_PrimInfo[] = {
    gl.TRIANGLES,
    gl.TRIANGLE_STRIP,
    gl.LINES,
    gl.LINE_STRIP,
    gl.POINTS,
};

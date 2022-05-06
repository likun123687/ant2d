#include <tuple>
#include <vector>
#include <gfx/bk/buffer.h>
#include <gfx/bk/texture.h>
#include <gfx/bk/uniform.h>
#include <gfx/bk/shader.h>

namespace bk {
    const uint16_t InvalidId = 0x0000;
    const uint16_t UInt16Max = 0xFFFF;

    const uint16_t IdMask = 0x0FFF;
    const uint16_t IdTypeShift = 12;

    enum IdType {
        IdTypeIndex = 0,
        IdTypeVertex,
        IdTypeTexture,
        IdTypeLayout,
        IdTypeUniformblock,
        IdTypeShader,
    };

    const uint16_t MaxIndex   = 2 << 10;
    const uint16_t MaxVertex  = 2 << 10;
    const uint16_t MaxTexture = 1 << 10;
    const uint16_t MaxUniformblock = 32 * 8;
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
    };

    class ResManager
    {
        private:
            std::array<IndexBuffer, MaxIndex> indexBuffers_;
            std::array<VertexBuffer, MaxVertex> vertexBuffers_;
            std::array<Texture2D, MaxTexture> textures_;

            std::array<Uniformblock, MaxUniformblock> uniformsblocks_;
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
            ResManager();
            std::tuple<uint16_t, IndexBuffer*> AllocIndexBuffer(const void *ptr, size_t size);
            std::tuple<uint16_t, VertexBuffer*>AllocVertexBuffer(const void *ptr, size_t size, uint16_t stride);
            std::tuple<uint16_t, Uniformblock*> AllocUniformblock(uint16_t shId, sg_shader_stage stage, const std::string& name);
            std::tuple<uint16_t, Texture2D*> AllocTexture(const ImageData& data);
            void Free(uint16_t id);
            IndexBuffer *GetIndexBuffer(uint16_t id);
            VertexBuffer *GetVertexBuffer(uint16_t id);
            Texture2D *GetTexture(uint16_t id);
            Uniformblock *GetUniformblock(uint16_t id);
            Shader *GetShader(uint16_t id);
    };
}

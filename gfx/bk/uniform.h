#include <string>
#include <array>
#include <asset/shader_utils.h>

namespace bk {
    const uint16_t UNIFORM_BLOCK_BUFFER__SIZE = 16 << 10;

    class Uniformblock
    {
        private:
            uint8_t slot_;
            uint8_t size_;
            sg_shader_stage stage_;
            ShaderType shader_type_;
            std::string name_;
        public:
            uint8_t Create(ShaderType type, sg_shader_stage stage, const std::string& name);
            static void decode(uint32_t code, uint8_t *stage, uint8_t *slot, uint8_t *size);
            static uint32_t encode(uint8_t stage, uint8_t slot, uint8_t size);
    };

    class UniformblockBuffer
    {
        public:
            UniformblockBuffer();
            uint32_t GetPos();
            void Reset();
            void Seek(uint32_t pos);
            bool IsEmpty();
            void WriteUInt32(uint32_t value);
            void WriteUInt16(uint16_t value);
            void Copy(const void *ptr, uint32_t size);
            void *ReadPointer(uint32_t size);

        private:
            std::array<uint8_t, UNIFORM_BLOCK_BUFFER__SIZE> buffer_;
            uint32_t size_;
            uint32_t pos_;
    };
}

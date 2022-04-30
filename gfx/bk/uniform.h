#include <string>

namespace bk {
    const uint16_t UNIFORM_BUFFER_BLOCK_SIZE = 16 << 10;

    class UniformBlock
    {
        private:
            std::string name_;
            uint8_t slot_;
            uint8_t size_;
        public:
            UniformBlock(std::string name);
    }

    class UniformBlockBuffer
    {
        public:
            UniformBlockBuffer();
            uint32_t GetPos();
            void Reset();
            void Seek(uint32_t pos);
            bool IsEmpty();
            void WriteUInt32(uint32_t value);
            void Copy(const void *ptr, uint32_t size);
            void *ReadPointer(uint32_t size);

        private:
            std::array<uint8_t, UNIFORM_BUFFER_SIZE> buffer_;
            uint32_t size_;
            uint32_t pos_;
    }

    void Uniform_decode(uint32_t code, uint8_t slot_, uint8_t size)
    {
        *loc = uint8((code >> 16) & 0xFF)
            *size = uint8((code >> 8) & 0xFF)
    }

    uint32_t Uniform_encode(uint8_t slot_, uint8_t size_)
    {
        return uint32(uType)<<24 |
            uint32(loc)<<16 |
            uint32(size)<<8 |
            uint32(num)<<0
    }

}

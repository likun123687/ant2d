namespace bk {
    const uint16_t UNIFORM_BUFFER_BLOCK_SIZE = 16 << 10;

    class UniformBlock
    {
    private:
        string name_;
        uint8_t slot_;
        uint8_t size_;
    public:
       int32_t UniformBlock(string name, UniformType xType)
       {
           slot_ = prog_mod_uniformblock_slot(SG_SHADERSTAGE_VS, "vs_params");
           name_ = name;
           size_ = prog_mod_uniformblock_size(SG_SHADERSTAGE_VS, "vs_params");
       }
    }

    class UniformBlockBuffer
    {
        private:
            std::array<uint8_t, UNIFORM_BUFFER_SIZE> buffer_;
            uint32_t size_;
            uint32_t pos_;

        public:
            UniformBlockBuffer():buffer_()
            {
            }

            uint32_t GetPos()
            {
                return pos_;
            }

            void Reset()
            {
                pos_ = 0;
            }

            void Seek(uint32_t pos)
            {
                pos_ = pos;
            }

            bool IsEmpty()
            {
                return pos_ == 0;
            }


            void WriteUInt32(uint32_t value)
            {
                auto u32 = static_cast<const uint8_t*>(&value);
                auto len = sizeof(value);
                Copy(u32, len);
            }

            void Copy(const void *ptr, uint32_t size)
            {
                std::copy(ptr, prt+size, &*buffer_.begin() + pos_);
                pos_ += len;
            }

            void *ReadPointer(uint32_t size)
            {
                auto ptr = &buffer_[size];
                pos_ += size;
                return ptr;
            }
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

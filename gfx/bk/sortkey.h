namespace bk {
    const uint32_t SK_LayerMask   = 0xF0000000;
    const uint32_t SK_ShaderMask  = 0x0F000000;
    const uint32_t SK_BlendMask   = 0x00F00000;
    const uint32_t SK_TextureMask = 0x000FF000;

    class SortKey
    {
        public:
            uint64_t Encode() {
                return 0 |
                    static_cast<uint64_t>(layer_)<<28 |
                    static_cast<uint64_t>(order_)<<18 |
                    static_cast<uint64_t>(shader_)<<13 |
                    static_cast<uint64_t>(blend_)<<10 |
                    static_cast<uint64_t>(texture_);
            }

            void Decode(uint64_t key) {
                texture_ = static_cast<uint16_t>((key >> 00) & (1<<10 - 1));
                blend_   = static_cast<uint16_t>((key >> 10) & (1<< 3 - 1));
                shader_  = static_cast<uint16_t>((key >> 13) & (1<< 5 - 1));
                order_   = static_cast<uint16_t>((key >> 18) & (1<<10 - 1));
                layer_   = static_cast<uint16_t>((key >> 28) & (1<< 4 - 1));
            }


        private:
            uint16_t layer_;
            uint16_t order_;
            uint16_t shader_;
            uint16_t blend_;
            uint16_t texture_;
    }
}

namespace bk {
    class IndexBuffer
    {
        public:
            void Create(uint32_t size, void *data, uint16_t flags)
            {
                size_ = size;
                flags_ = flags;

                sg_buffer_desc desc = {};
                desc.type = SG_BUFFERTYPE_INDEXBUFFER;
                if (data) {
                    desc.usage = SG_USAGE_IMMUTABLE;
                    desc.data.ptr = data;
                    desc.data.size = size_;
                } else {
                    desc.usage = SG_USAGE_DYNAMIC;
                    desc.size = size;
                }
                id_ = sg_make_buffer(desc);
                static_assert(id_);
            }

            void Update(uint32_t offset, uint32_t size, void *data, bool discard)
            {
                static_assert(id_);
                if (discard) {
                    Destroy();
                    Create(size, data, flags_);
                }
                sg_range ran = {};
                ran.ptr = data;
                ran.size = size;
                sg_update_buffer(id_, &ran);
            }

            void Destroy()
            {
                sg_destroy_buffer(id_);
            }

        private:
            sg_buffer id_;
            uint32_t size_;
            uint16_t flags;
    }


    class VertexBuffer {
        public:
            void Create(uint32_t size, void *data, uint16_t layout,uint16_t flags)
            {
                size_ = size;
                layout_ = layout;

                sg_buffer_desc desc = {};
                if (data) {
                    desc.usage = SG_USAGE_IMMUTABLE;
                    desc.data.ptr = data;
                    desc.data.size = size_;
                } else {
                    desc.usage = SG_USAGE_DYNAMIC;
                    desc.size = size;
                }
                id_ = sg_make_buffer(desc);
                static_assert(id_);
            }

            void Update(uint32_t offset, uint32_t size, void *data, bool discard)
            {
                static_assert(id_);
                if (discard) {
                    Destroy();
                    Create(size_, nullptr,layout_, 0);
                }
                sg_range ran = {};
                ran.ptr = data;
                ran.size = size;
                sg_update_buffer(id_, &ran);
            }

        private:
            sg_buffer id_;
            uint32_t size_;
            uint16_t layout_;
    }

}

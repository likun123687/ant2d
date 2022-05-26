#include <gfx/bk/buffer.h>
#include <cassert>

using namespace ant2d;

void IndexBuffer::Create(const uint8_t *data, size_t size, uint16_t flags)
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
    id_ = sg_make_buffer(&desc);
    assert(id_.id != SG_INVALID_ID);
}

void IndexBuffer::Update(const uint8_t *data, size_t size, size_t offset, bool discard)
{
    assert(id_.id != SG_INVALID_ID);
    if (discard) {
        Destroy();
        Create(nullptr, size, flags_);
    }
    sg_range ran = {data, size};
    //ran.ptr = data;
    //ran.size = size;
    sg_update_buffer(id_, &ran);
}

void IndexBuffer::Destroy()
{
    sg_destroy_buffer(id_);
    id_.id = SG_INVALID_ID;
}

sg_buffer IndexBuffer::GetId() const
{
    return id_;
}

void VertexBuffer::Create(const uint8_t *data, size_t size, uint16_t layout, uint16_t flags)
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
    id_ = sg_make_buffer(&desc);
    assert(id_.id != SG_INVALID_ID);
}

void VertexBuffer::Update(const uint8_t *data, size_t size, size_t offset, bool discard)
{
    assert(id_.id != SG_INVALID_ID);
    if (discard) {
        Destroy();
        Create(nullptr, size_, layout_, 0);
    }
    sg_range ran = {};
    ran.ptr = data;
    ran.size = size;
    sg_update_buffer(id_, &ran);
}

void VertexBuffer::Destroy()
{
    sg_destroy_buffer(id_);
    id_.id = SG_INVALID_ID;
}

sg_buffer VertexBuffer::GetId() const
{
    return id_;
}

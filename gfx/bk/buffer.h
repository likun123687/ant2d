#pragma once

#include <cstdint>
#include <gfx/bk/sokol_gfx_bk.h>

namespace bk {

class IndexBuffer
{
public:
    void Create(const void *data, size_t size, uint16_t flags);
    void Update(const void *data, size_t size, size_t offset, bool discard);
    void Destroy();

private:
    sg_buffer id_;
    uint32_t size_;
    uint16_t flags_;
};

class VertexBuffer
{
public:
    void Create(const void *data, size_t size, uint16_t layout, uint16_t flags);
    void Update(const void *data, size_t size, size_t offset, bool discard);
    void Destroy();

private:
    sg_buffer id_;
    size_t size_;
    uint16_t layout_;
};
} //namespace bk

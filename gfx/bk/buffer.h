#pragma once

#include <cstdint>
#include <gfx/bk/sokol_gfx_bk.h>

namespace bk {

class IndexBuffer
{
public:
    void Create(uint32_t size, void *data, uint16_t flags);
    void Update(uint32_t offset, uint32_t size, void *data, bool discard);
    void Destroy();

private:
    sg_buffer id_;
    uint32_t size_;
    uint16_t flags_;
};

class VertexBuffer
{
public:
    void Create(uint32_t size, void *data, uint16_t layout,uint16_t flags);
    void Update(uint32_t offset, uint32_t size, void *data, bool discard);
    void Destroy();

private:
    sg_buffer id_;
    uint32_t size_;
    uint16_t layout_;
};
} //namespace bk

#pragma once

#include <cstdint>
#include <gfx/bk/sokol_gfx_bk.h>

namespace ant2d {

class IndexBuffer {
public:
    void Create(const uint8_t* data, size_t size, uint16_t flags);
    void Update(const uint8_t* data, size_t size, size_t offset, bool discard);
    void Destroy();
    sg_buffer GetId() const;

private:
    sg_buffer id_;
    uint32_t size_;
    uint16_t flags_;
};

class VertexBuffer {
public:
    void Create(const uint8_t* data, size_t size, uint16_t layout, uint16_t flags);
    void Update(const uint8_t* data, size_t size, size_t offset, bool discard);
    void Destroy();
    sg_buffer GetId() const;

private:
    sg_buffer id_;
    size_t size_;
    uint16_t layout_;
};
} // namespace ant2d

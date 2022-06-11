#pragma once

namespace ant2d {
namespace bk {
void Init();
void Reset(uint16_t width, uint16_t height, float pixel_ratio);
void Destroy();
void SetState(uint64_t state, uint32_t rgba);
void SetIndexBuffer(uint16_t id, uint16_t first_index, uint16_t num);
void SetVertexBuffer(uint8_t stream, uint16_t id);
void SetTexture(uint8_t stage, uint16_t tex_id);
void SetUniformblock(uint16_t id, uint8_t* ptr);
void SetStencil(uint32_t stencil);
uint16_t SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void SetScissorCached(uint16_t id);
void SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height);
void SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height);
void SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth,
    uint8_t stencil);
uint32_t Submit(uint8_t id, uint16_t shader, uint64_t depth);
int Flush();
} //namespace ant2d
} //namespace bk

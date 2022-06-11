#include <gfx/bk/res_manager.h>
#include <gfx/bk/queue.h>

namespace ant2d {
namespace bk {

void Init()
{
}

void Reset(uint16_t width, uint16_t height, float pixel_ratio)
{
    SharedRenderQueue.Reset(width, height, pixel_ratio);
}

void Destroy()
{
    //SharedResManager.Destroy();
    //SharedRenderQueue.Destroy();
}

void SetState(uint64_t state, uint32_t rgba)
{
    SharedRenderQueue.SetState(state, rgba);
}

void SetIndexBuffer(uint16_t id, uint16_t first_index, uint16_t num)
{
    SharedRenderQueue.SetIndexBuffer(id, first_index, num);
}

void SetVertexBuffer(uint8_t stream, uint16_t id)
{
    SharedRenderQueue.SetVertexBuffer(stream, id);
}

// SetSprite sets texture stages for drawCall primitive.
void SetTexture(uint8_t stage, uint16_t tex_id) {
    SharedRenderQueue.SetTexture(stage, tex_id);
}

void SetUniformblock(uint16_t id, uint8_t* ptr)
{
    SharedRenderQueue.SetUniformblock(id, ptr);
}

void SetStencil(uint32_t stencil)
{
    SharedRenderQueue.SetStencil(stencil);
}

uint16_t SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    return SharedRenderQueue.SetScissor(x, y, width, height);
}

// SetScissorCached set scissor rect with a cached ScissorRect handler.
void SetScissorCached(uint16_t id)
{
    SharedRenderQueue.SetScissorCached(id);
}

// SetViewScissor view scissor. Draw primitive outsize view will be clipped. When
// x, y, with, height are set to 0, scissor will be disabled.
void SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height)
{
    SharedRenderQueue.SetViewScissor(id, x, y, width, height);
}

void SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width,
        uint16_t height)
{
    SharedRenderQueue.SetViewPort(id, x, y, width, height);
}

// Set view clear Flags. rgba is Color clear value(default = 0x000000ff), depth is Depth clear value
// (default = 1.0), stencil is Stencil clear value(default = 0).
void SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth,
    uint8_t stencil)
{
    SharedRenderQueue.SetViewClear(id, flags, rgba, depth, stencil);
}

// Submit primitive for rendering. Default depth is zero.
// Returns Number of drawCall calls.
uint32_t Submit(uint8_t id, uint16_t shader, uint64_t depth)
{
    return SharedRenderQueue.Submit(id, shader, depth);
}

// Execute final draw
int Flush()
{
    return SharedRenderQueue.Flush();
}
} //namespace ant2d
} //namespace bk

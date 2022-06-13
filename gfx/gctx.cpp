#include <gfx/gctx.h>
#include <gfx/bk/bk.h>
#include <gfx/bk/res_manager.h>
#include <algorithm>

namespace ant2d {
namespace gfx{

void Init(float pixel_ratio)
{
    ::ant2d::bk::Init();
    ::ant2d::bk::Reset(480, 320, pixel_ratio);
}

int Flush()
{
    auto num = ::ant2d::bk::Flush();
    SharedContext.Step();
    return num;
}

void Destroy()
{
    ::ant2d::bk::Destroy();
    SharedContext.Destroy();
}

std::tuple<uint16_t, int, VertexBuffer*> Context::TempVertexBuffer(int req_size, int stride)
{
    Tempbuffer buffer;
    bool found = false;

    for (auto &tb:temps_) {
        if (tb.used == 0 && tb.stride == stride && tb.size >= req_size) {
            tb.used = 1;
            buffer = tb;
            found = true;
            break;
        }
    }

    if (!found) {
        buffer = NewVertexBuffer(req_size, stride);
        temps_.push_back(buffer);
    }

    return std::make_tuple(buffer.id, buffer.size, buffer.vb);
}

Tempbuffer Context::NewVertexBuffer(int vertex_size, int stride)
{
    {
        //对齐
        vertex_size--;
        vertex_size |= vertex_size >> 1;
        vertex_size |= vertex_size >> 2;
        vertex_size |= vertex_size >> 3;
        vertex_size |= vertex_size >> 8;
        vertex_size |= vertex_size >> 16;
        vertex_size++;
    }

    Tempbuffer tb = {};
    tb.size = vertex_size;
    tb.stride = stride;
    tb.used = 1;

    uint16_t id = kInvalidId;
    VertexBuffer *vb = nullptr;

    std::tie(id, vb) = SharedResManager.AllocVertexBuffer(nullptr, vertex_size*stride, stride);
    if (id != kInvalidId) {
        tb.id = id;
        tb.vb = vb;
    }
    return tb;
}

void  Context::Release()
{
    for (auto &tb: temps_) {
        tb.used = 0;
    }
    std::sort(temps_.begin(), temps_.end(),
    [](const Tempbuffer & a, const Tempbuffer & b) -> bool
    {
        return a.size < b.size;
    });
}

void Context::Step()
{
    Release();
}

void Context::Destroy()
{
}

// 64kb, format={3, 0, 1, 3, 1, 2}
std::tuple<uint16_t, int> Context::SharedIndexBuffer()
{
    if (shared_.index.empty()) {
        InitIndexBuffer();
    }
    return std::make_tuple(shared_.id, shared_.size);
}

void Context::InitIndexBuffer()
{
    shared_.index.resize(kSharedIndexBufferSize);
    uint16_t i_format[6] = {3, 0, 1, 3, 1, 2};
    for (int i = 0; i < kSharedIndexBufferSize; i+=6) {
        std::copy(i_format, i_format + 6, shared_.index.begin() + i);
        i_format[0] += 4;
        i_format[1] += 4;
        i_format[2] += 4;
        i_format[3] += 4;
        i_format[4] += 4;
        i_format[5] += 4;
    }

    uint16_t id = 0;
    std::tie(id, std::ignore) = SharedResManager.AllocIndexBuffer((uint8_t *)(shared_.index.data()), kSharedIndexBufferSize * 2);
    if (id != kInvalidId) {
        shared_.id = id;
        shared_.size = kSharedIndexBufferSize;
    }
};
} //namespace gfx
} //namespace ant2d

#include <gfx/bk/queue.h>
using namespace bk;

RenderQueue::RenderQueue(ResManager *R):ub_(new UniformBuffer()), 
        ctx_(new RenderContext(R, ub_.get())), rm_(R)
{
}

void RenderQueue::Init()
{
    ctx_.Init();
}

void RenderQueue::Reset(uint16_t w, uint16_t h, float pr)
{
    ctx_.wRect_.w = w;
    ctx_.wRect_.h = h;
    ctx_.wRect_.pixelRatio_ = pr;
}

void RenderQueue::SetState(uint64_t state, uint32_t rgba)
{
    drawCall_.state_ = state;
}

void RenderQueue::SetIndexBuffer(uint16_t id, uint16_t firstIndex, uint16_t num)
{
    drawCall_.indexBuffer_ = id & IdMask;
    drawCall_.firstIndex_ = firstVertex;
    drawCall_.num_ = num;
}

void RenderQueue::SetVertexBuffer(uint8_t stream, uint16_t id, uint16_t firstVertex, uint16_t numVertex)
{
    if (stream < 0 || stream >= 2) {
        //todo err handle
        return;
    }
    auto vbStream = &drawCall_.vertexBuffers_[stream];
    vbStream->vertexBuffers_ = id & IdMask;
    vbStream->vertexFormat_ = InvalidId;
    vbStream->firstVertex_ = firstVertex;
    vbStream->numVertex_ = numVertex;
}

void RenderQueue::SetTexture(uint8_t stage, uint16_t samplerId, uint16_t texId, uint32_t flags)
{
    if (stage < 0 || stage >= 2) {
        log.Printf("Not suppor texture location: %d", stage)
        //todo err handle
        return
    }

    drawCall_.textures_[stage] = texId & IdMask;
}

void RenderQueue::SetUniform(uint16_t id, void *ptr)
{
    auto um = rm_.Uniform(id);
    if (um) {
        auto opcode = Uniform_encode(um->Type, um->Slot, um->Size, um->Count);
        ub_.WriteUInt32(opcode);
        ub_.Copy(ptr, static_cast<uint32_t>(um->Size) * static_cast<uint32_t>(um->Count));
    }
}

uint32_t RenderQueue::SetStencil(uint32_t stencil)
{
    drawCall_.stencil_ = stencil;
}

uint16_t RenderQueue::SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint16_t id = ctx_.AddClipRect(x, y, width, height);
    drawCall_.scissor_ = id;
    return id;
}

void RenderQueue::SetScissorCached(uint16_t id)
{
    drawCall_.scissor_ = id;
}

void RenderQueue::SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (id < 0 || id >= 4) {
        //log.Printf("Not support view id: %d", id)
        return;
    }
    scissors_[id] = Rect{x, y, with, height};
}

void RenderQueue::SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (id < 0 || id >= 4) {
        //log.Printf("Not support view id: %d", id)
        return;
    }
    viewports_[id] = Rect{x, y, with, height};
}


void RenderQueue::SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth, uint8_t stencil)
{
    if (id < 0 || id >= 4) {
        log.Printf("Not support view id: %d", id)
        return;
    }

    auto clear = &clears_[id];
    clear.flags_ = flags;
    clear.rgba_ = rgba;
    clear.depth_ = depth;
    clear.stencil_ = stencil;
}

uint32_t RenderQueue::Submit(uint8_t id, uint16_t program, uint64_t depth)
{
    uniformEnd_ = s_cast<uint16_t>(ub_.GetPos());
    sk_.Layer = s_cast<uint16_t>(id);
    sk_.Order = s_cast<uint16_t>(depth+0xFFFF>>1);

    sk_.Shader = program & IdMask;
    sk_.Blend = 0;
    sk_.Texture = drawCall_.textures[0];

    sortKeys_[drawCallNum_] = sk_.Encode();
    sortValues_[drawCallNum_] = drawCallNum_;

    drawCall_.uniformBegin = uniformBegin_;
    drawCall_.uniformEnd_ = uniformEnd_;

    drawCallList_[drawCallNum_] = drawCall_;
    drawCallNum_++;

    drawCall_.reset();
    uniformBegin_ = static_cast<uint16_t>(ub_.GetPos());
    return 0;
}

int RenderQueue::Flush()
{
    auto num = drawCallNum_;

    auto sortKeys = {sortKeys_.begin(), sortKeys_.begin() + num};
    auto sortValues = {sortValues_.begin(), sortValues_.begin() + num};
    auto drawList = {drawCallList_.begin(), drawCallList_.begin() + num};

    auto cmp = [](const auto &x, const auto &y) {
        return std::get<0>(x) > std::get<0>(y);
    };
    auto kv_zip = Zip(sortKeys, sortValues);
    switch (sortmode_) {
        case Ascending:
            break;
        case Descending:
            cmp = [](const auto &x, const auto &y) {
                return std::get<0>(x) < std::get<0>(y);
            };
            break;
    }

    ctx_.Draw(sortKeys, sortValues, drawList);

    drawCallNum_ = 0;
    uniformBegin_ = 0;
    uniformEnd_ = 0;
    ub_.Reset();
    ctx_.Reset();
}

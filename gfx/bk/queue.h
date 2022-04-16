namespace bk {

enum SortMode {
    Sequential,
    Ascending,
    Descending,
}

class Rect {
    private:
        uint16 x_;
        uint16 y_;
        uint16 w_;
        uint16 h_;
    public:
        Rect(uint16 x, uint16 y, uint16 w, uint16 h):x_(x), y_(y), w_(w), h_(h)
        {
        }
        void clear()
        {
            x_ = 0;
            y_ = 0;
            w_ = 0;
            h_ = 0;
        }

        bool isZero()
        {
            return x_==0 && y_==0 && w_==0 && h_== 0;
        }

}

struct Stream
{
    uint16_t vertexBuffer_;
    uint16_t vertexFormat_;
    uint16_t firstVertex_;
}

class RenderDraw
{
public:
    void reset()
    {
        indexBuffer_ = 0;
        firstIndex_ = 0;
        num_ = 0;
        scissor_ = 0;
    }
private:
    uint16_t indexBuffer_;
    std::array<Stream, 2> vertexBuffers_;
    std::array<uint16_t, 2> textures_;

    uint16_t firstIndex_;
    uint16_t num_;

    uint16_t uniformBegin_;
    uint16_t uniformEnd_;

    uint32_t stencil_;
    uint16_t scissor_;

    uint64_t state_;

}

const uint16_t MAX_QUEUE_SIZE = 8 << 10;

class RenderQueue
{
private:
    SortMode sortmode_;
    std::array<uint64_t, MAX_QUEUE_SIZE> sortKeys_;
    std::array<uint16_t, MAX_QUEUE_SIZE> sortValues_;
    std::array<RenderDraw, MAX_QUEUE_SIZE> drawCallList_;
    uint16_t drawCallNum_;
    SortKey sk_;
    RenderDraw drawCall_;
    uint16_t uniformBegin_;
    uint16_t uniformEnd_;

    std::array<Rect, 4> viewports_;
    std::array<Rect, 4> scissor_;

    std::array<struct {
            std::array<uint8_t, 8> index_;
            uint32_t rgba_;
            float depth_;
            uint8_t stencil_;
            uint16_t flags_;
        },4> clears_;

    ResManager *rm_;
    std::unique_ptr<UniformBuffer> ub_;
    std::unique_ptr<RenderContext> ctx_;

public:
    RenderQueue(ResManager *R):ub_(new UniformBuffer()), 
        ctx_(new RenderContext(R, ub_.get())), rm_(R)
    {
    }

    void Init()
    {
        ctx_.Init();
    }


    void Reset(uint16_t w, uint16_t h, float pr)
    {
        ctx_.wRect_.w = w;
        ctx_.wRect_.h = h;
        ctx_.wRect_.pixelRatio_ = pr;
    }

    void SetState(uint64_t state, uint32_t rgba)
    {
        drawCall_.state_ = state;
    }

    void SetIndexBuffer(uint16_t id, uint16_t firstIndex, uint16_t num)
    {
        drawCall_.indexBuffer_ = id & IdMask;
        drawCall_.firstIndex_ = firstVertex;
        drawCall_.num_ = num;
    }

    void SetVertexBuffer(uint8_t stream, uint16_t id, uint16_t firstVertex, uint16_t numVertex)
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

    void SetTexture(uint8_t stage, uint16_t samplerId, uint16_t texId, uint32_t flags)
    {
        if (stage < 0 || stage >= 2) {
            log.Printf("Not suppor texture location: %d", stage)
            //todo err handle
            return
        }

        drawCall_.textures_[stage] = texId & IdMask;
    }

    void SetUniform(uint16_t id, void *ptr)
    {
        auto um = rm_.Uniform(id);
        if (um) {
            auto opcode = Uniform_encode(um->Type, um->Slot, um->Size, um->Count);
            ub_.WriteUInt32(opcode);
            ub_.Copy(ptr, static_cast<uint32_t>(um->Size) * static_cast<uint32_t>(um->Count));
        }
    }

    uint32_t SetStencil(uint32_t stencil)
    {
        drawCall_.stencil_ = stencil;
    }

    uint16_t SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        uint16_t id = ctx_.AddClipRect(x, y, width, height);
        drawCall_.scissor_ = id;
        return id;
    }

    void SetScissorCached(uint16_t id)
    {
        drawCall_.scissor_ = id;
    }

    void SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        if (id < 0 || id >= 4) {
            //log.Printf("Not support view id: %d", id)
            return;
        }
        scissors_[id] = Rect{x, y, with, height};
    }

    void SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    {
        if (id < 0 || id >= 4) {
            //log.Printf("Not support view id: %d", id)
            return;
        }
        viewports_[id] = Rect{x, y, with, height};
    }


    void SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth, uint8_t stencil)
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

    uint32_t Submit(uint8_t id, uint16_t program, uint64_t depth)
    {
        uniformEnd_ = static_cast<uint16_t>(ub_.GetPos());
        sk_.Layer = static_cast<uint16_t>(id);
        sk_.Order = static_cast<uint16_t>(depth+0xFFFF>>1);

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

    int Flush()
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

}



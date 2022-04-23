#include <cstdint>
#include <array>

namespace bk {

enum SortMode {
    Sequential,
    Ascending,
    Descending,
}

class Rect
{
    private:
        uint16_t x_;
        uint16_t y_;
        uint16_t w_;
        uint16_t h_;
    public:
        Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h):x_(x), y_(y), w_(w), h_(h)
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

};

struct Stream
{
    uint16_t vertexBuffer_;
    uint16_t vertexFormat_;
    uint16_t firstVertex_;
};

class RenderDraw
{
public:
    void reset()
    {
        indexBuffer = 0;
        firstIndex = 0;
        num = 0;
        scissor = 0;
    }
    uint16_t indexBuffer;
    std::array<Stream, 2> vertexBuffers;
    std::array<uint16_t, 2> textures;

    uint16_t firstIndex;
    uint16_t num;

    uint16_t uniformBegin;
    uint16_t uniformEnd;

    uint32_t stencil;
    uint16_t scissor;

    uint64_t state;
};

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
            std::array<uint8_t, 8> index;
            uint32_t rgba;
            float depth;
            uint8_t stencil;
            uint16_t flags;
        },4> clears_;

    ResManager *rm_;
    std::unique_ptr<UniformBuffer> ub_;
    std::unique_ptr<RenderContext> ctx_;

public:
    RenderQueue(ResManager *R);
    void Init();
    void Reset(uint16_t w, uint16_t h, float pr);
    void SetState(uint64_t state, uint32_t rgba);

    void SetIndexBuffer(uint16_t id, uint16_t firstIndex, uint16_t num);
    void SetVertexBuffer(uint8_t stream, uint16_t id, uint16_t firstVertex, uint16_t numVertex);
    void SetTexture(uint8_t stage, uint16_t samplerId, uint16_t texId, uint32_t flags);
    void SetUniform(uint16_t id, void *ptr);
    uint32_t SetStencil(uint32_t stencil);
    uint16_t SetScissor(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void SetScissorCached(uint16_t id);
    void SetViewScissor(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void SetViewPort(uint8_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void SetViewClear(uint8_t id, uint16_t flags, uint32_t rgba, float depth, uint8_t stencil);
    uint32_t Submit(uint8_t id, uint16_t program, uint64_t depth);
    int Flush();
}


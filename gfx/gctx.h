#pragma once
#include <gfx/bk/buffer.h>
#include <utils/singleton.h>
#include <vector>

// graphics context
// a wrapper for bk-api
namespace ant2d {
namespace gfx {
    constexpr uint16_t kSharedIndexBufferSize = static_cast<uint16_t>(0xFFFF / 6) * 6;

    void Init(float pixel_ratio);
    int Flush();
    void Destroy();

    // 目前各个 RenderFeature 都是自己管理 VBO/IBO，但是对于一些系统，比如
    // Batch/ParticleSystem(2D中的大部分元素)，都是可以复用VBO的，顶点数据
    // 需要每帧动态生成，如此可以把这些需要动态申请的Buffer在此管理起来，对应的
    // CPU 数据可以在 StackAllocator 上申请，一帧之后就自动释放。
    struct Tempbuffer {
        VertexBuffer* vb;
        int size;
        int stride;
        uint16_t id;
        uint16_t used;
    };

    class Context {
        struct Shared {
        public:
            Shared()
                : id { 0 }
                , padding { 0 }
                , index {}
                , size { 0 }
            {
            }
            uint16_t id;
            uint16_t padding;
            std::vector<uint16_t> index;
            int size;
        };

    public:
        Context();
        // 一帧之后自动释放
        std::tuple<uint16_t, int, VertexBuffer*> TempVertexBuffer(int req_size, int stride);
        Tempbuffer NewVertexBuffer(int vertex_size, int stride);
        void Release();
        void Step();
        void Destroy();
        std::tuple<uint16_t, int> SharedIndexBuffer();
        void InitIndexBuffer();

    private:
        Shared shared_;
        std::vector<Tempbuffer> temps_;
    };
} // namespace gfx
} // namespace ant2d

#define SharedContext \
    ::ant2d::Singleton<::ant2d::gfx::Context>()

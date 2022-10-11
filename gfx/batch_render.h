#pragma once
#include <gfx/render_data.h>
#include <gfx/camera.h>
#include <array>
#include <memory>

namespace ant2d {

// 管理一或多个Batch实例
// 最多可以生成 128 个 Batch 分组
// 最多可以使用 8 个 VBO 缓存
class BatchContext {
    // 目前采用提前申请好大块空间的方式，会导致大量的内存浪费
    // 之后可以把vbo管理起来，按需使用
    // ~ 640k per-batch, 32k vertex, 8k quad
    const uint32_t kMaxBatchQuadSize = uint32_t(8 << 10);
    const uint32_t kMaxBatchVertexSize = 4 * kMaxBatchQuadSize;
    using BatchList = std::array<Batch, 128>;

private:
    std::vector<PosTexColorVertex> vertex_;
    uint32_t vertex_pos_;
    uint32_t first_vertex_;

    // state
    int batch_used_;
    uint16_t tex_id_;
    int16_t depth_;
    BatchList batch_list_;

public:
    BatchContext();
    void Begin(uint16_t tex, int16_t depth);
    void DrawComp(IBatchObject* bo);
    void End();
    void Reset();
    void FlushBuffer();
    uint32_t GetVertexPos();
    BatchList& GetBatchList();
    int GetBatchUsed();

#ifdef ANT2D_DEBUG
    auto& GetVertex()
    {
        return vertex_;
    }

    auto& GetFirstVertex()
    {
        return first_vertex_;
    }

    auto& GetTexId()
    {
        return tex_id_;
    }

#endif
};

// Batch Render:
// Use PosTexColorVertex struct with P4C4 format
/// A Tex2D Batch TypeRender
class BatchRender : public IRender {
private:
    // uint64_t state_flags_;
    // uint32_t rgba_;
    //  shader program
    // uint16_t shader_id_;
    uint16_t pipeline_id_;
    // uniform handle
    uint16_t umh_projection_;
    // uint16_t umh_sampler0_;
    std::unique_ptr<BatchContext> batch_context_;

public:
    BatchRender(ShaderType shader_type);
    void SetCamera(Camera* camera);
    // submit all batched group
    void Submit(std::vector<Batch> b_list);
    void Begin(uint16_t tex, uint16_t depth);
    void Draw(IBatchObject* b);
    void End();
    int Flush();
#ifdef ANT2D_DEBUG
    BatchContext* GetBatchContext()
    {
        return batch_context_.get();
    }

#endif
};

} // namespace ant2d

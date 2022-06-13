#include <gfx/batch_render.h>
#include <gfx/bk/render_context.h>
#include <gfx/bk/res_manager.h>
#include <gfx/bk/bk.h>
#include <math/project.h>
#include <gfx/gctx.h>

namespace ant2d {
BatchRender::BatchRender(ShaderType shader_type):batch_context_(new BatchContext())
{
    shader_type_ = shader_type;
    // setup state
    state_flags_ |= kStateBlend.alpha_premultiplied;
    uint16_t sh_id = kInvalidId;
    Shader *sh = nullptr;
    std::tie(sh_id, sh) = SharedResManager.AllocShader(shader_type);
    if (sh_id != kInvalidId) {
        shader_id_ = sh_id;

        // setup attribute
        sh->AddAttributeBinding("xyuv", 0, SG_VERTEXFORMAT_FLOAT4);
        sh->AddAttributeBinding("rgba", 0, SG_VERTEXFORMAT_BYTE4);

        Uniformblock *uniformblock = nullptr;
        uint16_t uniformblock_id = 0;
        std::tie(uniformblock_id, uniformblock) = SharedResManager.AllocUniformblock(sh_id,SG_SHADERSTAGE_FS, "batch_vs_params");
        if (uniformblock_id != kInvalidId) {
            umh_projection_ = uniformblock_id;
        }
        bk::Submit(0, shader_id_, 0);
    }
}

void BatchRender::SetCamera(Camera *camera)
{
    float left = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float top = 0.0f;
    auto p = math::Ortho2D(left, right, bottom, top);
    bk::SetUniformblock(umh_projection_, (uint8_t*)(&p));
    bk::Submit(0, shader_id_, 0);
}

// submit all batched group
void BatchRender::Submit(std::vector<Batch> b_list)
{
    for (auto &b:b_list) {
        // state
        bk::SetState(state_flags_, rgba_);
        bk::SetTexture(0, b.texture_id);

        // set vertex
        bk::SetVertexBuffer(0, b.vertex_id);
        bk::SetIndexBuffer(b.index_id, uint32_t(b.first_index), uint32_t(b.num_index));

        // submit draw-call
        bk::Submit(0, shader_id_, int32_t(b.depth));
    }
}

void BatchRender::Begin(uint16_t tex, uint16_t depth)
{
    batch_context_->Begin(tex, depth);
}

void BatchRender::Draw(IBatchObject* b)
{
    batch_context_->DrawComp(b);
}

void BatchRender::End()
{
    batch_context_->End();
}

int BatchRender::Flush()
{
    auto &bc = batch_context_;
    if (bc->GetVertexPos() > 0) {
        bc->FlushBuffer();
    }
    auto b_list = bc->GetBatchList();
    auto batch_used = bc->GetBatchUsed();
    std::vector<Batch> submit_list = {b_list.begin(), b_list.begin()+batch_used};
    this->Submit(submit_list);
    return batch_used;
}

BatchContext::BatchContext()
{
    vertex_.resize(kMaxBatchVertexSize);
}

void BatchContext::Begin(uint16_t tex_id, int16_t depth)
{
    tex_id_ = tex_id;
    depth_ = depth;
    first_vertex_ = vertex_pos_;
}

// 计算世界坐标并保存到 Batch 结构
//
//   3 ---- 2
//   | `    |
//   |   `  |
//   0------1
void BatchContext::DrawComp(IBatchObject *bo)
{
    uint32_t step = bo->Size();
    if (vertex_pos_ + step > kMaxBatchVertexSize) {
        FlushBuffer();
        End();
        vertex_pos_ = 0;
        first_vertex_ = 0;
    }

    std::vector<PosTexColorVertex> buf = {vertex_.begin(), vertex_.begin() + step};
    vertex_pos_ += step;
    bo->Fill(buf);
}

// commit a batch
void BatchContext::End()
{
    if (batch_used_ > 128) {
        Error("Batch List out of size:({}, {}) ", 128, batch_used_);
    }

    auto &batch = batch_list_[batch_used_];
    batch.texture_id = tex_id_;
    batch.depth = depth_;

    batch.vertex_id = kInvalidId;
    batch.first_vertex = 0; //uint16(bc.firstVertex)
    batch.num_vertex = uint16_t(vertex_pos_- first_vertex_);
    batch.first_index = uint16_t(first_vertex_/4 * 6);
    batch.num_index = uint16_t(batch.num_vertex/4 * 6);
    batch_used_ += 1;
}

// upload buffer
void BatchContext::Reset()
{
    tex_id_ = 0;
    first_vertex_ = 0;
    vertex_pos_ = 0;
    batch_used_ = 0;
}

// flushBuffer() will write and switch vertex-buffer
// we must submit batch with a end() method
void BatchContext::FlushBuffer()
{
    auto req_size = vertex_pos_;
    auto stride = sizeof(PosTexColorVertex);

    uint16_t iid = 0;
    std::tie(iid, std::ignore) = SharedContext.SharedIndexBuffer();

    uint16_t vid = 0;
    VertexBuffer *vb = nullptr;
    std::tie(vid, std::ignore, vb) = SharedContext.TempVertexBuffer(req_size, stride);
    vb->Update(reinterpret_cast<uint8_t*>(vertex_.data()), vertex_pos_ * stride, 0, false);

    for (int i = batch_used_; i >= 0; i--) {
        auto &b = batch_list_[i];
        if (b.vertex_id == kInvalidId) {
            b.vertex_id = vid;
            b.index_id = iid;
        }
    }
}

uint32_t BatchContext::GetVertexPos()
{
    return vertex_pos_;
}

BatchContext::BatchList& BatchContext::GetBatchList()
{
    return batch_list_;
}

int BatchContext::GetBatchUsed()
{
    return batch_used_;
}

}

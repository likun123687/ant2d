#include <gfx/dbg/debug_render.h>
#include <utils/debug.h>

namespace ant2d {
DebugRender::DebugRender(ShaderType shader_type)
    : buffer_ { 2048 * 4 }
{
    shader_type_ = shader_type;
    uint16_t sh_id = kInvalidId;
    Shader* sh = nullptr;
    std::tie(sh_id, sh) = SharedResManager.AllocShader(shader_type);

    sg_pipeline_desc pdesc = {};
    pdesc.shader = sh->GetShdId();
    pdesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT4;
    pdesc.layout.attrs[1].format = SG_VERTEXFORMAT_UBYTE4N;
    pdesc.index_type = SG_INDEXTYPE_UINT16;
    pdesc.colors->blend.enabled = true;
    pdesc.colors->blend.src_factor_rgb = SG_BLENDFACTOR_ONE;
    pdesc.colors->blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    pdesc.colors->blend.src_factor_alpha = SG_BLENDFACTOR_ONE;
    pdesc.colors->blend.dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    // make pipeline
    uint16_t pipeline_id = 0;
    Pipeline* pipeline = nullptr;
    std::tie(pipeline_id, pipeline) = SharedResManager.AllocPipeline(&pdesc);
    pipeline_id_ = pipeline_id;

    std::tie(umh_projection_, std::ignore) = SharedResManager.AllocUniformblock(sh_id, SG_SHADERSTAGE_VS, "debug_vs_proj");
    if (umh_projection_ == kInvalidId) {
        Error("alloc uniform block error");
    }
}

void DebugRender::Destroy()
{
    SharedResManager.Free(pipeline_id_);
    SharedResManager.Free(umh_projection_);
}

void DebugRender::SetViewPort(float x, float y, float w, float h)
{
    view_.x = x;
    view_.y = y;
    view_.w = w;
    view_.h = h;

    auto left = x - view_.w / 2;
    auto right = x + view_.w / 2;
    auto bottom = y - view_.h / 2;
    auto top = y + view_.h / 2;
    auto p = math::Ortho2D(left, right, bottom, top);
    //auto p = math::Ortho2D(0, w, 0, h);

    debug_vs_proj_t vs_proj = { p };
    bk::SetUniformblock(umh_projection_, (uint8_t*)(&vs_proj));
    bk::Submit(0, pipeline_id_, dbg::MAX_ZORDER);
}

void DebugRender::Draw()
{
    bk::SetTexture(0, buffer_.GetFontTexId());
    // set vertex
    // bk::SetVertexBuffer(0, buffer_.GetVertexId(), buffer_.GetPos() * sizeof(PosTexColorVertex));
    bk::SetVertexBuffer(0, buffer_.GetVertexId());
    bk::SetIndexBuffer(buffer_.GetIndexId(), 0, (buffer_.GetPos() * 6) >> 2);
    bk::Submit(0, pipeline_id_, dbg::MAX_ZORDER);
}

DebugRender::View& DebugRender::GetView()
{
    return view_;
}

TextShapeBuffer& DebugRender::GetBuffer()
{
    return buffer_;
}

} // namespace ant2d

#include <gfx/mesh/mesh_render.h>
#include <utils/debug.h>
#include <gfx/camera.h>
#include <math/project.h>

namespace ant2d {
MeshRender::MeshRender(ShaderType shader_type)
{
    shader_type_ = shader_type;
    uint16_t sh_id = kInvalidId;
    Shader* sh = nullptr;
    std::tie(sh_id, sh) = SharedResManager.AllocShader(shader_type);

    sg_pipeline_desc pdesc = {};
    pdesc.shader = sh->GetShdId();
    // setup attribute
    // sh->AddAttributeBinding("xyuv", 0, SG_VERTEXFORMAT_FLOAT4);
    // sh->AddAttributeBinding("rgba", 0, SG_VERTEXFORMAT_BYTE4);
    pdesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT4;
    pdesc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
    pdesc.index_type = SG_INDEXTYPE_UINT16;

    // make pipeline
    uint16_t pipeline_id = 0;
    Pipeline* pipeline = nullptr;
    std::tie(pipeline_id, pipeline) = SharedResManager.AllocPipeline(&pdesc);
    pipeline_id_ = pipeline_id;

    std::tie(umh_projection_, std::ignore) = SharedResManager.AllocUniformblock(sh_id, SG_SHADERSTAGE_VS, "mesh_vs_proj");
    if (umh_projection_ == kInvalidId) {
        Error("alloc uniform block error");
    }

    std::tie(umh_model_, std::ignore) = SharedResManager.AllocUniformblock(sh_id, SG_SHADERSTAGE_VS, "mesh_vs_model");
    if (umh_model_ == kInvalidId) {
        Error("alloc uniform block error");
    }
}

void MeshRender::SetCamera(Camera* camera)
{
    auto [left, right, bottom, top] = camera->P();
    auto p = math::Ortho2D(left, right, bottom, top);

    mesh_vs_proj_t vs_proj = { p };
    bk::SetUniformblock(umh_projection_, (uint8_t*)(&vs_proj));
    bk::Submit(0, pipeline_id_, 0);
}

void MeshRender::Extract(std::vector<uint32_t> visible_objects)
{
}

// draw
void MeshRender::Draw(Mesh* m, math::Mat4* mat4, int32_t depth)
{
    bk::SetTexture(0, m->GetTexture());

    // set uniform - mvp
    bk::SetUniformblock(umh_model_, (uint8_t*)mat4);

    // set vertex
    bk::SetVertexBuffer(0, m->GetVertexId());
    bk::SetIndexBuffer(m->GetIndexId(), m->first_index_, m->num_index_);
    bk::Submit(0, pipeline_id_, depth);
}

}
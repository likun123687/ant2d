#include <gfx/mesh/mesh_render_feature.h>
#include <utils/debug.h>

namespace ant2d {
void MeshRenderFeature::Register(RenderSystem* rs)
{
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kMeshShader) {
            mesh_render_ = reinterpret_cast<MeshRender*>(r.get());
        }
    }

    for (auto& t : (*rs->GetTableList())) {
        if (t->GetTableType() == TableType::KMesh) {
            mesh_table_ = reinterpret_cast<MeshTable*>(t.get());
        } else if (t->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(t.get());
        }
    }

    id_ = rs->Accept(this);
}

void MeshRenderFeature::Extract(View* v)
{
    auto camera = v->camera;
    auto fi = uint32_t(id_) << 16;
    for (int i = 0; i < mesh_table_->GetSize(); i++) {
        auto mesh = mesh_table_->GetComp(i);
        auto transform = transform_table_->GetComp(mesh->GetEntity());
        auto [width, height] = mesh->Size();
        if (mesh->GetVisible() && camera->InView(transform, math::Vec2 { width, height }, math::Vec2 { 0.5, 0.5 })) {
            auto z_order = mesh->GetZOrder();
            auto sid = PackSortId(z_order.GetValue(), 0);
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void MeshRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    auto mat4 = math::Mat4::Ident4();
    for (auto& b : nodes) {
        auto ii = b.value & 0xFFFF; // 在mesh_table索引
        auto mesh = mesh_table_->GetComp(ii);
        auto entity = mesh->GetEntity();
        auto transform = transform_table_->GetComp(entity);
        auto srt = transform->GetWorld();

        auto c = math::Cos(srt.rotation);
        auto s = math::Sin(srt.rotation);

        mat4[0] = c * srt.scale[0];
        mat4[1] = s * srt.scale[0];
        mat4[4] = -s * srt.scale[1];
        mat4[5] = c * srt.scale[1];

        mat4[12] = srt.position[0];
        mat4[13] = srt.position[1];

        mat4[10] = 1;
        mat4[15] = 1;

        mesh_render_->Draw(mesh->GetMesh(), &mat4, mesh->GetZOrder().GetValue());
    }
}

void MeshRenderFeature::Flush()
{
}

}
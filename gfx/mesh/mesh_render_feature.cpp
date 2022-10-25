#include <gfx/mesh/mesh_render_feature.h>

namespace ant2d {
void SpriteRenderFeature::Register(RenderSystem* rs)
{
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kBatchShader) {
            mesh_render_ = reinterpret_cast<MeshRender*>(r.get());
        }
    }

    for (auto& t : (*rs->GetTableList())) {
        if (t->GetTableType() == TableType::kParticle) {
            mesh_table_ = reinterpret_cast<MeshTable*>(t.get());
        } else if (t->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(t.get());
        }
    }

    id_ = rs->Accept(this);
}

void SpriteRenderFeature::Extract(View* v)
{
    auto camera = v->camera;
    auto fi = uint32_t(id_) << 16;
    for (int i = 0; i < mesh_table_->GetSize(); i++) {
        auto mesh = mesh_table_->GetComp(i);
        auto transform = transform_table_->GetComp(mesh->GetEntity());

        if (mesh->GetVisible() && camera->InView(transform, mesh->Size(), math::Vec2 { 0.5, 0.5 })) {
            auto z_order = mesh->GetZOrder();
            auto sid = PackSortId(z_order.GetValue(), 0);
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void SpriteRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    auto mat4 = math::Ident4();
    for (auto& b : nodes) {
        auto ii = b.value & 0xFFFF; // 在mesh_table索引
        auto mesh = mesh_table_->GetComp(ii);
        auto entity = mesh->GetEntity();
        auto transform = transform_table_->GetComp(entity);
        auto srt = transform->GetWorld();

        auto c = math::Cos(srt.rotation);
        auto s = math::Sin(srt.rotation);

        mat4[0] = c * srt.Scale[0];
        mat4[1] = s * srt.Scale[0];
        mat4[4] = -s * srt.Scale[1];
        mat4[5] = c * srt.Scale[1];
        mat4[8] = srt.Position[0];
        mat4[9] = srt.Position[1];

        mat4[10] = 1;
        mat4[15] = 1;

        mesh_render_->Draw(mesh->GetMesh(), &mat4, mesh->GetZOrder().GetValue());
    }
}

}
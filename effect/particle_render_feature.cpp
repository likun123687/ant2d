#include <effect/particle_render_feature.h>

namespace ant2d {

void ParticleRenderFeature::Register(RenderSystem* rs)
{
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kMeshShader) {
            mesh_render_ = reinterpret_cast<MeshRender*>(r.get());
            break;
        }
    }

    for (auto& t : (*rs->GetTableList())) {
        if (t->GetTableType() == TableType::kParticle) {
            particle_table_ = reinterpret_cast<ParticleSystemTable*>(t.get());
        } else if (t->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(t.get());
        }
    }

    id_ = rs->Accept(this);
}

void ParticleRenderFeature::Extract(View* v)
{
    auto camera = v->camera;
    auto fi = uint32_t(id_) << 16;
    for (int i = 0; i < particle_table_->GetSize(); i++) {
        auto pc = particle_table_->GetComp(i);
        auto transform = transform_table_->GetComp(pc->GetEntity());
        auto [width, height] = pc->GetSize();
        if (pc->Visible() && camera->InView(transform, math::Vec2 { width, height }, math::Vec2 { 0.5, 0.5 })) {
            auto z_order = pc->GetZOrder();
            auto sid = PackSortId(z_order, 0);
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void ParticleRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    Info("particle render feature draw aa");
    int require_vertex_size = 0;
    int require_index_size = 0;
    for (auto& node : nodes) {
        auto ii = node.value & 0xFFFF;
        auto particle = particle_table_->GetComp(ii);
        int cap = 0;
        std::tie(std::ignore, cap) = particle->GetSimulator()->Size();
        require_vertex_size += cap * 4;
        if (cap > require_index_size) {
            require_index_size = cap * 6;
        }
    }

    buffer_context_.AllocBuffer(require_vertex_size, require_index_size);
    Info("particle render feature draw bb");
    auto mesh = Mesh {};
    mesh.SetIndexId(buffer_context_.index_id_);
    mesh.SetVertexId(buffer_context_.vertex_id_);

    auto mat4 = math::Mat4 { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    int offset = 0;
    for (auto& node : nodes) {
        uint16_t z = 0;
        std::tie(z, std::ignore) = UnpackSortId(node.sort_id);
        auto ps = particle_table_->GetComp(node.value & 0xFFFF);
        auto entity = ps->GetEntity();
        auto xf = transform_table_->GetComp(entity);

        int live = 0;
        std::tie(live, std::ignore) = ps->GetSimulator()->Size();

        auto vsz = live * 4;
        auto isz = live * 6;

        std::vector<PosTexColorVertex> buff { std::begin(buffer_context_.vertex_) + offset, std::begin(buffer_context_.vertex_) + offset + vsz };
        ps->GetSimulator()->Visualize(buff, ps->GetTexture());
        mesh.SetFirstVertex(uint16_t(offset));
        offset += vsz;
        mesh.SetNumVertex(uint16_t(vsz));
        mesh.SetFirstIndex(0);
        mesh.SetNumIndex(uint16_t(isz));
        mesh.SetTexture(ps->GetTexture()->GetTextureId());

        auto p = xf->GetPosition();
        mat4.Set(0, 3, p[0]);
        mat4.Set(1, 3, p[1]);
        mesh_render_->Draw(&mesh, &mat4, z);

        stats_.lives += live;
    }
}

void ParticleRenderFeature::Flush()
{
    // TODO add dgb
    stats_.lives = 0;
}

}
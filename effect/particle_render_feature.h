#pragma once
#include <gfx/render_data.h>
#include <gfx/mesh/mesh_render.h>
#include <gfx/gctx.h>
#include <effect/particle_table.h>
#include <gfx/transform/transform.h>

namespace ant2d {

class BufferContext {
public:
    void AllocBuffer(int vertex_size, int index_size)
    {
        int sz = 0;
        std::tie(vertex_id_, sz, vb_) = SharedContext.TempVertexBuffer(vertex_size, sizeof(PosTexColorVertex));
        if (sz > vertex_size_) {
            vertex_size_ = sz;
            vertex_.resize(sz);
        }

        if (index_size > index_size_) {
            std::tie(index_id_, index_size_) = SharedContext.SharedIndexBuffer();
        }
    }
    void Release()
    {
    }

    uint16_t vertex_id_;
    uint16_t index_id_;
    int vertex_size_;
    int index_size_;
    std::vector<PosTexColorVertex> vertex_;
    VertexBuffer* vb_;
}

class ParticleRenderFeature : public IRenderFeature {
    struct Stats {
        int lives = 0;
    };

private:
    MeshRender* mesh_render_;
    int id_;
    BufferContext buffer_context_;
    ParticleTable* particle_table_;
    TransformTable* transform_table_;
    Stats stats_;

public:
    void Register(RenderSystem* rs)
    {
        for (auto& r : rs->GetRenderList()) {
            if (r->GetType() == ShaderType::kMeshShader) {
                mesh_render_ = reinterpret_cast<MeshRender*>(r.get());
                break;
            }
        }

        for (auto& t : (*rs->GetTableList())) {
            if (t->GetTableType() == TableType::kParticle) {
                particle_table_ = reinterpret_cast<ParticleTable*>(t.get());
            } else if (t->GetTableType() == TableType::kTransform) {
                transform_table_ = reinterpret_cast<TransformTable*>(t.get());
            }
        }

        id_ = rs->Accept(this);
    }

    void Extract(View* v)
    {
        auto camera = v->camera;
        auto fi = uint32_t(id_) << 16;
        for (int i = 0; i < particle_table_->GetSize(); i++) {
            auto pc = particle_table_->GetComp(i);
            auto transform = transform_table_->GetComp(pc->GetEntity());
            auto [width, height] = pc->Size();
            if (pc->GetVisible() && camera->InView(transform, math::Vec2 { width, height }, math::Vec2 { 0.5, 0.5 })) {
                auto z_order = mesh->GetZOrder();
                auto sid = PackSortId(z_order.GetValue(), 0);
                auto value = fi + uint32_t(i);
                v->render_nodes.push_back(SortObject { sid, value });
            }
        }
    }

    void Draw(const std::vector<SortObject>& nodes)
    {
        int require_vertex_size = 0;
        int require_index_size = 0;
        for (auto& node : nodes) {
            auto ii = node.value & 0xFFFF;
            auto particle = particle_table_->GetComp(ii);
            int cap = 0;
            std::tie(std::ignore, cap) = particle->GetSimulator()->Size();
            if (cap > require_index_size) {
                require_index_size = cap*6;
            }
        }

        buffer_context_.AllocBuffer(require_vertex_size, require_index_size);

        
    }
};

}

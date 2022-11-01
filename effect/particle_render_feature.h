#pragma once
#include <gfx/render_data.h>
#include <gfx/mesh/mesh_render.h>
#include <gfx/gctx.h>
#include <effect/particle_table.h>
#include <gfx/transform/transform.h>
#include <gfx/render_system.h>
#include <gfx/transform/transform_table.h>

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
};

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
    void Register(RenderSystem* rs);

    void Extract(View* v);

    void Draw(const std::vector<SortObject>& nodes);
    void Flush();
};

}

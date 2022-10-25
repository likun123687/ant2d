#pragma once
#include <gfx/render_data.h>
#include <gfx/mesh/mesh_render.h>
#include <gfx/mesh/mesh_table.h>
#include <gfx/transform/transform_table.h>
#include <math/common_func.h>
#include <gfx/mesh/mesh.h>

namespace ant2d {
class SpriteRenderFeature : public IRenderFeature {
private:
    int16_t id_;
    MeshRender* mesh_render_;
    MeshTable* mesh_table_;
    TransformTable* transform_table_;

public:
    void Register(RenderSystem* rs);
    void Extract(View* v);
    void Draw(const std::vector<SortObject>& nodes);
};
}
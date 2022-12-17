#pragma once
#include <gfx/render_data.h>
#include <gfx/mesh/mesh_render.h>
#include <gui/drawing.h>
#include <gui/gui.h>
#include <gfx/gctx.h>
#include <gfx/render_system.h>
#include <gfx/dbg/debug.h>

namespace ant2d {
class UIRenderFeature : public IRenderFeature {
private:
    struct Buffer {
        bool first_draw = true;
        uint16_t iid;
        uint16_t vid;
        int isz;
        int vsz;
        VertexBuffer* vertex;
        IndexBuffer* index;
    };

    int16_t id_;
    MeshRender* mesh_render_;
    DrawList* draw_list_;
    Camera* camera_;
    Buffer buffer_;

    void AllocBuffer(int isz, int vsz);

public:
    void SetDrawList(DrawList* dl);

    void Register(RenderSystem* rs);

    void Extract(View* v);

    void Draw(const std::vector<SortObject>& nodes);
    void Flush();
};
}

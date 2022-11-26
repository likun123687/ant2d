#pragma once
#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>
#include <gfx/dbg/text_shape_buffer.h>
#include <math/project.h>
#include <gfx/bk/bk.h>

namespace ant2d {
class DebugRender {
private:
    struct View {
        float x;
        float y;
        float w;
        float h;
    };
    uint32_t rgba_;

    View view_;
    ShaderType shader_type_;
    uint16_t pipeline_id_;

    // uniform handle
    uint16_t umh_projection_; // Projection
    TextShapeBuffer buffer_;

public:
    DebugRender(ShaderType shader_type);
    void Destroy();
    void SetViewPort(float x, float y, float w, float h);

    void Draw();

    View& GetView();

    TextShapeBuffer& GetBuffer();
};

} // namespace ant2d

#pragma once
#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>
#include <gfx/mesh/mesh.h>

namespace ant2d {
class MeshRender  {
private:
    uint32_t rgba_;
    struct {
        float x,
        float y,
        float w,
        float h
    } view_;
    uint16_t pipeline_id_;

    // uniform handle
    uint16_t umh_projection_; // Projection
    uint16_t umh_model_; // model
public:
    MeshRender(ShaderType shader_type);

};

} // namespace ant2d

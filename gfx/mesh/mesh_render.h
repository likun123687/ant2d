#pragma once
#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>
#include <gfx/mesh/mesh.h>

namespace ant2d {
class MeshRender : public IRender {
private:
    uint16_t pipeline_id_;

    // uniform handle
    uint16_t umh_projection_; // Projection
    uint16_t umh_model_; // model
public:
    MeshRender(ShaderType shader_type);

    void SetCamera(Camera* camera);

    void Extract(std::vector<uint32_t> visible_objects);

    // draw
    void Draw(Mesh* m, math::Mat4* mat4, int32_t depth);
};

} // namespace ant2d

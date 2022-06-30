#pragma once

#include <array>
#include <asset/shader_utils.h>
#include <gfx/bk/sokol_gfx_bk.h>
#include <string>

namespace ant2d {

class Shader {
private:
    sg_shader sg_shd_id_;
    ShaderType type_;

public:
    void Create(ShaderType type);
    ShaderType GetType();
    sg_shader GetShdId();
    void Destroy();
};

}

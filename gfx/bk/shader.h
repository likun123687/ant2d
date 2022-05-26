#pragma once

#include <gfx/bk/sokol_gfx_bk.h>
#include <asset/shader_utils.h>
#include <string>
#include <array>

namespace ant2d {

struct AttribBind {
    uint16_t slot_;   // slot location
    uint16_t stream_; // stream index
    sg_vertex_format vertex_format_;
};

class Shader {
    private:
        sg_shader sg_shd_id_;
        std::array<AttribBind, 32> attr_binds_;
        uint32_t num_attr_;
        ShaderType type_;

    public:
        void Create(ShaderType type);
        void AddAttributeBinding(std::string attr, uint32_t stream, sg_vertex_format vertex_format);
        ShaderType GetType();
        sg_shader GetShdId();
        void Destroy();

        uint32_t GetNumAttr() const
        {
            return num_attr_;
        }

        AttribBind GetAttribBind(int i) const
        {
            return attr_binds_[i];
        }
};

}

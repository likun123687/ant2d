#pragma once

#include <gfx/bk/sokol_gfx_bk.h>
#include <asset/shader_utils.h>
#include <string>
#include <array>

namespace bk {

typedef struct AttribBind{
    uint16_t slot;   // slot location
    uint16_t stream; // stream index
    sg_vertex_format vertex_format;
} AttribBind;

class Shader {
    private:
        sg_shader sg_shd_id_;
        std::array<AttribBind, 32> AttrBinds_;
        uint32_t numAttr_;
        ShaderType type_;

    public:
        void Create(ShaderType type);
        void AddAttributeBinding(std::string attr, uint32_t stream, sg_vertex_format vertex_format);
        ShaderType GetType();
        void Destroy();
};

}

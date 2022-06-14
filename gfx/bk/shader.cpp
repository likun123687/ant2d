#include <gfx/bk/shader.h>
#include <utils/debug.h>

using namespace ant2d;
void Shader::Create(ShaderType type)
{
    auto shader_desc_func = GetShaderDescFunc(type);
    sg_shd_id_ = sg_make_shader(shader_desc_func(sg_query_backend()));
    type_ = type;
}

void Shader::AddAttributeBinding(std::string attr, uint32_t stream, sg_vertex_format vertex_format)
{
    auto func = GetAttrSlotFunc(type_);
    int slot = func(attr.c_str());
    if (slot < 0) {
        Error("fail to bind attribute: {}, {}", attr, vertex_format);
        return;
    }

    auto bind = &attr_binds_[num_attr_];
    bind->slot_ = static_cast<uint16_t>(slot);
    bind->stream_ = static_cast<uint16_t>(stream);
    bind->vertex_format_ = vertex_format;
    num_attr_++;
}

ShaderType Shader::GetType()
{
    return type_;
}

void Shader::Destroy()
{
    sg_destroy_shader(sg_shd_id_);
}

sg_shader Shader::GetShdId()
{
    return sg_shd_id_;
}

uint32_t Shader::GetNumAttr() const
{
    return num_attr_;
}

AttribBind Shader::GetAttribBind(int i) const
{
    return attr_binds_[i];
}

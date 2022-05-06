#include <gfx/bk/shader.h>
#include <utils/Debug.h>

using namespace bk;
void Shader::Create(ShaderType type)
{
    auto shader_desc_func = GetShaderDescFunc(type);
    sg_shd_id_ = sg_make_shader(shader_desc_func(sg_query_backend()));
    type_ = type;
}

void Shader::AddAttributeBinding(std::string attr,  uint32_t stream,  sg_vertex_format vertex_format)
{
    auto func = GetAttrSlotFunc(type_);
    int slot = func(attr.c_str());
    if (slot < 0) {
        Error("fail to bind attribute: {}, {}", attr, vertex_format);
        return;
    }

    auto bind = &AttrBinds_[numAttr_];
    bind->slot = static_cast<uint16_t>(slot);
    bind->stream = static_cast<uint16_t>(stream);
    bind->vertex_format = vertex_format;
    numAttr_++;
}

ShaderType Shader::GetType()
{
    return type_;
}

void Shader::Destroy()
{
    sg_destroy_shader(sg_shd_id_);
}

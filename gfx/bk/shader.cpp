#include <gfx/bk/shader.h>
#include <utils/debug.h>

using namespace ant2d;
void Shader::Create(ShaderType type)
{
    auto shader_desc_func = GetShaderDescFunc(type);
    sg_shd_id_ = sg_make_shader(shader_desc_func(sg_query_backend()));
    type_ = type;
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

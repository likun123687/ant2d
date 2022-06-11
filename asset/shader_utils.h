#pragma once

#include <asset/shdc.h>

namespace ant2d {

using shader_desc_func = const sg_shader_desc* (*)(sg_backend backend);
using attr_slot_func = int (*)(const char* attr_name);
using uniformblock_slot_func = int (*)(sg_shader_stage stage, const char* ub_name);
using uniformblock_size_func = size_t (*)(sg_shader_stage stage, const char* ub_name);

enum ShaderType {
    kBatchShader = 0,
    kMeshShader,
};

shader_desc_func GetShaderDescFunc(ShaderType type);
attr_slot_func GetAttrSlotFunc(ShaderType type);
uniformblock_slot_func GetUniformblockSlotFunc(ShaderType type);
uniformblock_size_func GetUniformblockSizeFunc(ShaderType type);

}

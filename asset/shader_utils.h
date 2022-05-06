#pragma once

#include <asset/shdc.h>

namespace bk {

typedef const sg_shader_desc* (*shader_desc_func)(sg_backend backend);
typedef int (*attr_slot_func)(const char* attr_name);
typedef int (*uniformblock_slot_func)(sg_shader_stage stage, const char* ub_name);
typedef size_t (*uniformblock_size_func)(sg_shader_stage stage, const char* ub_name);

enum ShaderType {
    kBatch,
    kMesh,
};

shader_desc_func GetShaderDescFunc(ShaderType type);
attr_slot_func GetAttrSlotFunc(ShaderType type);
uniformblock_slot_func GetUniformblockSlotFunc(ShaderType type);
uniformblock_size_func GetUniformblockSizeFunc(ShaderType type);

}

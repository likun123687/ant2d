#include <asset/shader_utils.h>
using namespace bk;

shader_desc_func bk::GetShaderDescFunc(ShaderType type)
{
    static shader_desc_func desc_func_list[kMesh+1];
    static bool valid;
    if (!valid) {
      valid = true;
      desc_func_list[kBatch] = &batch_shader_desc;
      desc_func_list[kMesh] = &mesh_shader_desc;
    }
    return desc_func_list[type];
}

attr_slot_func bk::GetAttrSlotFunc(ShaderType type)
{
    static attr_slot_func attr_slot_func_list[kMesh+1];
    static bool valid;
    if (!valid) {
      valid = true;
      attr_slot_func_list[kBatch] = &batch_attr_slot;
      attr_slot_func_list[kMesh] = &mesh_attr_slot;
    }
    return attr_slot_func_list[type];
}

uniformblock_slot_func bk::GetUniformblockSlotFunc(ShaderType type)
{
    static uniformblock_slot_func uniformblock_slot_func_list[kMesh+1];
    static bool valid;
    if (!valid) {
      valid = true;
      uniformblock_slot_func_list[kBatch] = &batch_uniformblock_slot;
      uniformblock_slot_func_list[kMesh] = &mesh_uniformblock_slot;
    }
    return uniformblock_slot_func_list[type];
}

uniformblock_size_func bk::GetUniformblockSizeFunc(ShaderType type)
{
    static uniformblock_size_func uniformblock_size_func_list[kMesh+1];
    static bool valid;
    if (!valid) {
      valid = true;
      uniformblock_size_func_list[kBatch] = &batch_uniformblock_size;
      uniformblock_size_func_list[kMesh] = &mesh_uniformblock_size;
    }
    return uniformblock_size_func_list[type];
}

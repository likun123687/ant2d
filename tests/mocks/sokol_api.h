#include <gfx/bk/sokol_gfx_bk.h>
#include <trompeloeil.hpp>

class sokol_gfx_api
{
public:
    MAKE_MOCK1(sg_make_buffer, sg_buffer(const sg_buffer_desc* desc));
    MAKE_MOCK2(sg_update_buffer, void(sg_buffer buf, const sg_range* data));
    MAKE_MOCK1(sg_destroy_buffer, void(sg_buffer buf));

    MAKE_MOCK2(sg_update_image, void(sg_image img, const sg_image_data* data));
    MAKE_MOCK0(sg_alloc_image, sg_image(void));
    MAKE_MOCK1(sg_destroy_image, void(sg_image img));
    MAKE_MOCK2(sg_init_image, void(sg_image img_id, const sg_image_desc* desc));

    MAKE_MOCK1(sg_make_shader, sg_shader(const sg_shader_desc* desc));
    MAKE_MOCK0(sg_query_backend, sg_backend (void));
    MAKE_MOCK1(sg_destroy_shader, void(sg_shader buf));

    //shader
    MAKE_MOCK1(batch_shader_desc, const sg_shader_desc*(sg_backend backend));
    MAKE_MOCK1(batch_attr_slot, int(const char* attr_name));
    MAKE_MOCK2(batch_image_slot, int(sg_shader_stage stage, const char* img_name));
    MAKE_MOCK2(batch_uniformblock_slot, int (sg_shader_stage stage, const char* ub_name));
    MAKE_MOCK2(batch_uniformblock_size, size_t (sg_shader_stage stage, const char* ub_name));
    MAKE_MOCK3(batch_uniform_offset, int (sg_shader_stage stage, const char* ub_name, const char* u_name));
    MAKE_MOCK3(batch_uniform_desc, sg_shader_uniform_desc (sg_shader_stage stage, const char* ub_name, const char* u_name));

    MAKE_MOCK1(mesh_shader_desc, const sg_shader_desc*(sg_backend backend));
    MAKE_MOCK1(mesh_attr_slot, int(const char* attr_name));
    MAKE_MOCK2(mesh_image_slot, int(sg_shader_stage stage, const char* img_name));
    MAKE_MOCK2(mesh_uniformblock_slot, int (sg_shader_stage stage, const char* ub_name));
    MAKE_MOCK2(mesh_uniformblock_size, size_t (sg_shader_stage stage, const char* ub_name));
    MAKE_MOCK3(mesh_uniform_offset, int (sg_shader_stage stage, const char* ub_name, const char* u_name));
    MAKE_MOCK3(mesh_uniform_desc, sg_shader_uniform_desc (sg_shader_stage stage, const char* ub_name, const char* u_name));
};


extern sokol_gfx_api sokol_gfx_api_mock;

#pragma once
#include <gfx/bk/sokol_gfx_bk.h>
#include <trompeloeil.hpp>
#include "sokol_app.h"
#include "sokol_glue.h"


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
    MAKE_MOCK1(sg_destroy_shader, void(sg_shader shd));

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
    MAKE_MOCK3(sg_apply_uniforms, void (sg_shader_stage stage, int ub_index, const sg_range* data));
    MAKE_MOCK5(sg_apply_viewport, void (int x, int y, int width, int height, bool origin_top_left));
    MAKE_MOCK3(sg_draw, void (int base_element, int num_elements, int num_instances));

    MAKE_MOCK1(sg_make_pipeline, sg_pipeline (const sg_pipeline_desc* desc));
    MAKE_MOCK3(sg_begin_default_pass, void (const sg_pass_action* pass_action, int width, int height));
    MAKE_MOCK1(sg_apply_pipeline, void (sg_pipeline pip));
    MAKE_MOCK1(sg_apply_bindings, void (const sg_bindings* bindings));
    MAKE_MOCK0(sg_end_pass, void (void));
    MAKE_MOCK0(sg_commit, void (void));
    MAKE_MOCK0(sapp_width, int (void));
    MAKE_MOCK0(sapp_widthf, float (void));
    MAKE_MOCK0(sapp_height, int (void));
    MAKE_MOCK0(sapp_heightf, float (void));
    MAKE_MOCK0(sapp_sgcontext, sg_context_desc (void));
    MAKE_MOCK1(sg_setup, void (const sg_desc* desc));
    MAKE_MOCK1(sg_destroy_pipeline, void(sg_pipeline pip));
};


extern sokol_gfx_api sokol_gfx_api_mock;

#include <tests/mocks/sokol_api.h>

sokol_gfx_api sokol_gfx_api_mock;

sg_buffer sg_make_buffer(const sg_buffer_desc* desc)
{
    return sokol_gfx_api_mock.sg_make_buffer(desc);
}

void sg_update_buffer(sg_buffer buf, const sg_range* data)
{
    return sokol_gfx_api_mock.sg_update_buffer(buf, data);
}

void sg_destroy_buffer(sg_buffer buf)
{
    return sokol_gfx_api_mock.sg_destroy_buffer(buf);
}

void sg_update_image(sg_image img, const sg_image_data* data)
{
    return sokol_gfx_api_mock.sg_update_image(img, data);
}

sg_image sg_alloc_image(void)
{
    return sokol_gfx_api_mock.sg_alloc_image();
}

void sg_destroy_image(sg_image img)
{
    return sokol_gfx_api_mock.sg_destroy_image(img);
}

void sg_init_image(sg_image img_id, const sg_image_desc* desc)
{
    return sokol_gfx_api_mock.sg_init_image(img_id, desc);
}


sg_shader sg_make_shader(const sg_shader_desc* desc)
{
    return sokol_gfx_api_mock.sg_make_shader(desc);
}

sg_backend sg_query_backend(void)
{
    return sokol_gfx_api_mock.sg_query_backend();
}

void sg_destroy_shader(sg_shader shd)
{
    return sokol_gfx_api_mock.sg_destroy_shader(shd);
}

const sg_shader_desc* batch_shader_desc(sg_backend backend)
{
    return sokol_gfx_api_mock.batch_shader_desc(backend);
}

int batch_attr_slot(const char* attr_name)
{
    return sokol_gfx_api_mock.batch_attr_slot(attr_name);
}

int batch_image_slot(sg_shader_stage stage, const char* img_name)
{
    return sokol_gfx_api_mock.batch_image_slot(stage, img_name);
}

int batch_uniformblock_slot(sg_shader_stage stage, const char* ub_name)
{
    return sokol_gfx_api_mock.batch_uniformblock_slot(stage, ub_name);
}

size_t batch_uniformblock_size(sg_shader_stage stage, const char* ub_name)
{
    return sokol_gfx_api_mock.batch_uniformblock_size(stage, ub_name);
}

int batch_uniform_offset(sg_shader_stage stage, const char* ub_name, const char* u_name)
{
    return sokol_gfx_api_mock.batch_uniform_offset(stage, ub_name, u_name);
}

sg_shader_uniform_desc batch_uniform_desc(sg_shader_stage stage, const char* ub_name, const char* u_name)
{
    return sokol_gfx_api_mock.batch_uniform_desc(stage, ub_name, u_name);
}

//
//
const sg_shader_desc* mesh_shader_desc(sg_backend backend)
{
    return sokol_gfx_api_mock.mesh_shader_desc(backend);
}

int mesh_attr_slot(const char* attr_name)
{
    return sokol_gfx_api_mock.mesh_attr_slot(attr_name);
}

int mesh_image_slot(sg_shader_stage stage, const char* img_name)
{
    return sokol_gfx_api_mock.mesh_image_slot(stage, img_name);
}

int mesh_uniformblock_slot(sg_shader_stage stage, const char* ub_name)
{
    return sokol_gfx_api_mock.mesh_uniformblock_slot(stage, ub_name);
}

size_t mesh_uniformblock_size(sg_shader_stage stage, const char* ub_name)
{
    return sokol_gfx_api_mock.mesh_uniformblock_size(stage, ub_name);
}

int mesh_uniform_offset(sg_shader_stage stage, const char* ub_name, const char* u_name)
{
    return sokol_gfx_api_mock.mesh_uniform_offset(stage, ub_name, u_name);
}

sg_shader_uniform_desc mesh_uniform_desc(sg_shader_stage stage, const char* ub_name, const char* u_name)
{
    return sokol_gfx_api_mock.mesh_uniform_desc(stage, ub_name, u_name);
}


void sg_apply_uniforms(sg_shader_stage stage, int ub_index, const sg_range* data)
{
    return sokol_gfx_api_mock.sg_apply_uniforms(stage, ub_index, data);
}

void sg_apply_viewport(int x, int y, int width, int height, bool origin_top_left)
{
    return sokol_gfx_api_mock.sg_apply_viewport(x, y, width, height, origin_top_left);
}

void sg_draw(int base_element, int num_elements, int num_instances)
{
    return sokol_gfx_api_mock.sg_draw(base_element, num_elements, num_instances);
}

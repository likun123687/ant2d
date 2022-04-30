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

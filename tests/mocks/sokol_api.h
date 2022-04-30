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
};

extern sokol_gfx_api sokol_gfx_api_mock;

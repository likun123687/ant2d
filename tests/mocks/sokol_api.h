#include <gfx/bk/sokol_gfx_bk.h>
#include <trompeloeil.hpp>
void sg_destroy_buffer(sg_buffer buf);

class sokol_gfx_api
{
public:
    MAKE_MOCK1(sg_make_buffer, sg_buffer(const sg_buffer_desc* desc));
    MAKE_MOCK2(sg_update_buffer, void(sg_buffer buf, const sg_range* data));
    MAKE_MOCK1(sg_destroy_buffer, void(sg_buffer buf));
};

extern sokol_gfx_api sokol_gfx_api_mock;

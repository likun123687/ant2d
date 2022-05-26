#include <cinttypes>
#include <gfx/bk/texture.h>
#include <utils/Content.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

using namespace ant2d;

TEST_CASE("testTexture")
{
    using trompeloeil::_; // wild card for matching any value

    sg_image image_id;
    image_id.id = 1;

    auto file_content = SharedContent.loadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);

    trompeloeil::sequence seq;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
        .IN_SEQUENCE(seq)
        .RETURN(image_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
        .LR_WITH((_1.id == image_id.id && _2->width == image_data.width))
        .LR_WITH(_2->data.subimage[0][0].size == image_data.width * image_data.height * 4)
        .IN_SEQUENCE(seq);

    auto texture = bk::Texture2D();
    texture.Create(image_data);
}

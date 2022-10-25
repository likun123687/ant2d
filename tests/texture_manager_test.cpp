#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>
#include <asset/texture_manager.h>
#include <utils/debug.h>

using namespace ant2d;

TEST_CASE("test_texture_manager")
{
    using trompeloeil::_; // wild card for matching any value
    trompeloeil::sequence seq;

    sg_image image_id;
    image_id.id = 1;
    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
        .IN_SEQUENCE(seq)
        .RETURN(image_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
        .IN_SEQUENCE(seq);

    auto texture_manager = TextureManager {};
    texture_manager.Load("assets/face.png");
    auto& repo = texture_manager.GetRepo();
    texture_manager.Load("assets/face.png");

    REQUIRE(repo["assets/face.png"].cnt == 2);

    //////
    uint16_t tex_id = 0;
    Texture2D* tex = nullptr;
    std::tie(tex_id, tex) = texture_manager.GetRaw("assets/face.png");
    REQUIRE(tex->GetId().id == 1);
    //////

    texture_manager.Unload("assets/face.png");
    REQUIRE(repo["assets/face.png"].cnt == 1);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_destroy_image(_))
        .IN_SEQUENCE(seq);
    texture_manager.Unload("assets/face.png");
    REQUIRE(repo.find("assets/face.png") == repo.end());

    // atlas test
    sg_image image_id1;
    image_id1.id = 2;
    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
        .IN_SEQUENCE(seq)
        .RETURN(image_id1);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
        .IN_SEQUENCE(seq);
    texture_manager.LoadAtlas("assets/test.png", "assets/test.json");
    auto atlas = texture_manager.GetAtlas("assets/test.png");
    REQUIRE(atlas != nullptr);

    REQUIRE(SharedAtlasManager.GetAtlasByName("assets/test.png") == atlas);
    auto sub_tex = atlas->GetSubTexByName("obj_deco006.png");
    auto size = sub_tex->GetSize();
    REQUIRE((size.width == 198 && size.height == 198));

    auto region = sub_tex->GetRegion();
    REQUIRE((region.x1 == 0 && std::fabs(region.x2 - float(198.0 / 636)) <= 1e-6));
}
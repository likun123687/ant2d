#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

#include <gfx/camera.h>
#include <gfx/transform/transform.h>

using namespace ant2d;

TEST_CASE("test_camera")
{
    auto camera = Camera{};
    camera.Init();
    auto size = math::Vec2{20, 20};
    auto gravity = math::Vec2{0.5, 0.5};

    auto trans = new Transform();
    trans->SetWorld(SRT{math::Vec2{1,1}, 0.0f, math::Vec2{300,300}});
    REQUIRE(camera.InView(trans, size, gravity));
}

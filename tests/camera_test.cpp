#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

#include <gfx/camera.h>

using namespace ant2d;

TEST_CASE("test_camera")
{
    auto camera = Camera{};
    camera.Init();
}

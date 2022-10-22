#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>
#include <asset/texture_manager.h>

using namespace ant2d;

TEST_CASE("test_texture_manager")
{
    auto texture_manager = TextureManager {};
    texture_manager.Load("assets/face.png");
}
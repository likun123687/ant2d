#include <asset/image.h>
#include <utils/content.h>
#include <utils/debug.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace ant2d;

TEST_CASE("test_image")
{
    auto file_content = SharedContent.loadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);
    REQUIRE((image_data.width == 512 && image_data.height == 512
        && image_data.size == 40855 && image_data.num_channels == 4));
}

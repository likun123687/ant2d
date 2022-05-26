#include <gfx/bk/texture.h>
#include <cinttypes>
#include <iostream>
#include <filesystem>
#include <utils/content.h>
#include <gfx/bk/buffer.h>
#include <utils/debug.h>
#include <gfx/bk/shader.h>
#include <gfx/bk/uniformblock.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

using namespace ant2d;

TEST_CASE("test_content")
{
    namespace fs = std::filesystem;
    auto asset_path = SharedContent.GetAssetPath();
    std::cout << "asset path "<< asset_path <<std::endl;

    auto file_not_exist = "assets/face.png333";
    REQUIRE( SharedContent.GetFullPath(file_not_exist) == std::string() );

    auto file_exist = "assets/face.png";
    auto full_path_file_exist = SharedContent.GetFullPath(file_exist);
    REQUIRE(!full_path_file_exist.empty());

    auto test_path = fs::path(asset_path) / file_exist;
    REQUIRE(fs::path(full_path_file_exist) == test_path);

#ifdef _WIN32
    REQUIRE(SharedContent.IsAbsolutePath("C:\\testfile.txt") == true);
#else
    REQUIRE(SharedContent.IsAbsolutePath("/abc/abc.txt") == true);
#endif
    REQUIRE(SharedContent.IsAbsolutePath("testfile\\aa\\bb.txt") == false);

    REQUIRE(SharedContent.IsFileExist(file_exist) == true);
    REQUIRE(SharedContent.IsFileExist(file_not_exist) == false);

    auto file_content = SharedContent.LoadFile("assets/file1.txt");
    auto result = std::string(reinterpret_cast<char *>(file_content.first.get()), file_content.second);
    auto expect_result = std::string("111122223333");
    REQUIRE(result == expect_result);
}

TEST_CASE("test_buffer")
{
    using trompeloeil::_;  // wild card for matching any value

    uint32_t size = 64;
    auto buffer = new uint8_t[static_cast<size_t>(size)];
    auto index_buffer = IndexBuffer();

    sg_buffer_desc desc = {};
    desc.type = SG_BUFFERTYPE_INDEXBUFFER;
    desc.usage = SG_USAGE_IMMUTABLE;
    desc.data.ptr = buffer;
    desc.data.size = size;

    sg_buffer buffer_id;
    buffer_id.id = 1;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_buffer(_))
    .WITH(_1->data.ptr == buffer && _1->data.size == size)
    .RETURN(buffer_id);

    index_buffer.Create(buffer, size,0);
}

TEST_CASE("test_texture")
{
    using trompeloeil::_;  // wild card for matching any value

    sg_image image_id;
    image_id.id = 1;

    auto file_content = SharedContent.LoadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);

    trompeloeil::sequence seq;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
    .IN_SEQUENCE(seq)
    .RETURN(image_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
    .LR_WITH((_1.id == image_id.id && _2->width == image_data.width_))
    .LR_WITH(_2->data.subimage[0][0].size == image_data.width_ * image_data.height_ * 4)
    .IN_SEQUENCE(seq);

    auto texture = Texture2D();
    texture.Create(image_data);
}

TEST_CASE("test_debug")
{
    Info("i ame a info message {}--{}", "message1", "message2");
    Warn("i ame a warn message");
    //Error("i ame a Error message");
    REQUIRE(1==1);
}

TEST_CASE("test_image_data")
{
    auto file_content = SharedContent.LoadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);
    REQUIRE((image_data.width_ == 512 && image_data.height_ == 512 \
            && image_data.size_ == 40855 && image_data.num_channels_ == 4));
}

TEST_CASE("test_shader")
{
    auto shader = Shader();
    shader.Create(kBatch);
}

TEST_CASE("test_uniform")
{
    auto uniformblock = Uniformblock();
    uniformblock.Create(kBatch, SG_SHADERSTAGE_FS, "aaa");
}

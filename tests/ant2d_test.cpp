#include <cinttypes>
#include <filesystem>
#include <gfx/bk/buffer.h>
#include <gfx/bk/shader.h>
#include <gfx/bk/texture.h>
#include <gfx/bk/uniformblock.h>
#include <iostream>
#include <utils/content.h>
#include <utils/debug.h>
#include <gfx/bk/sort_key.h>
#include <gfx/bk/res_manager.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

using namespace ant2d;

TEST_CASE("test_content")
{
    namespace fs = std::filesystem;
    auto asset_path = SharedContent.GetAssetPath();
    std::cout << "asset path " << asset_path << std::endl;

    auto file_not_exist = "assets/face.png333";
    REQUIRE(SharedContent.GetFullPath(file_not_exist) == std::string());

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
    auto result = std::string(reinterpret_cast<char*>(file_content.first.get()), file_content.second);
    auto expect_result = std::string("111122223333");
    REQUIRE(result == expect_result);
}

TEST_CASE("test_buffer")
{
    using trompeloeil::_; // wild card for matching any value

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

    index_buffer.Create(buffer, size, 0);
}

TEST_CASE("test_texture")
{
    using trompeloeil::_; // wild card for matching any value

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
    // Error("i ame a Error message");
    REQUIRE(1 == 1);
}

TEST_CASE("test_image_data")
{
    auto file_content = SharedContent.LoadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);
    REQUIRE((image_data.width_ == 512 && image_data.height_ == 512
        && image_data.size_ == 40855 && image_data.num_channels_ == 4));
}

TEST_CASE("test_shader")
{
    using trompeloeil::_; // wild card for matching any value
    trompeloeil::sequence seq;

    auto shader = Shader();
    sg_shader shid = sg_shader{1};
    sg_shader_desc sh_desc = {};
    sh_desc.label = "batch_shader_unique";

    {
        REQUIRE_CALL(sokol_gfx_api_mock, sg_query_backend())
            .IN_SEQUENCE(seq)
            .RETURN(SG_BACKEND_D3D11);

        REQUIRE_CALL(sokol_gfx_api_mock, batch_shader_desc(_))
            .IN_SEQUENCE(seq)
            .LR_WITH((_1 == SG_BACKEND_D3D11))
            .RETURN(&sh_desc);

        REQUIRE_CALL(sokol_gfx_api_mock, sg_make_shader(_))
            .IN_SEQUENCE(seq)
            .LR_WITH((std::string(_1->label) == std::string(sh_desc.label)))
            .RETURN(shid);
        shader.Create(kBatch);
        REQUIRE(shader.GetType() == kBatch);
        REQUIRE(shader.GetShdId().id == shid.id);
    }

    {
        REQUIRE_CALL(sokol_gfx_api_mock, batch_attr_slot(_))
            .IN_SEQUENCE(seq)
            .LR_WITH(std::string(_1) == std::string("aaa"))
            .RETURN(1);
        shader.AddAttributeBinding("aaa", 0, SG_VERTEXFORMAT_FLOAT3);
    }

    {
        REQUIRE_CALL(sokol_gfx_api_mock, sg_destroy_shader(_))
            .IN_SEQUENCE(seq)
            .LR_WITH(_1.id == shid.id);

        shader.Destroy();
    }

}

TEST_CASE("test_uniform")
{
    using trompeloeil::_; // wild card for matching any value
    trompeloeil::sequence seq;

    auto uniformblock = Uniformblock();

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_slot(_,_))
        .IN_SEQUENCE(seq)
        .LR_WITH(_1 == SG_SHADERSTAGE_FS && std::string(_2) == std::string("aaa"))
        .RETURN(1);

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_size(_,_))
        .IN_SEQUENCE(seq)
        .LR_WITH(_1 == SG_SHADERSTAGE_FS && std::string(_2) == std::string("aaa"))
        .RETURN(22);

    uniformblock.Create(kBatch, SG_SHADERSTAGE_FS, "aaa");
    REQUIRE(uniformblock.GetSlot() == 1);
    REQUIRE(uniformblock.GetSize() == 22);
    REQUIRE(uniformblock.GetStage() == SG_SHADERSTAGE_FS);
    REQUIRE(uniformblock.GetShaderType() == kBatch);




}

TEST_CASE("test_sort_key")
{
    SortKey sort_key =  SortKey();
    sort_key.layer_ = 1;
    sort_key.order_ = 2;
    sort_key.shader_ = 3;
    sort_key.blend_ = 4;
    sort_key.texture_ = 5;

    auto key = sort_key.Encode();
    REQUIRE(key == 0x10087005);

    auto sort_key1 = SortKey();
    sort_key1.Decode(key);

    bool ret = sort_key1.layer_ == 1 && \
    sort_key1.order_ == 2 && \
    sort_key1.shader_ == 3 && \
    sort_key1.blend_ == 4 && \
    sort_key1.texture_ == 5;

    REQUIRE(ret);

    sort_key.layer_ = 9;
    sort_key.order_ = 8;
    sort_key.shader_ = 7;
    sort_key.blend_ = 1;
    sort_key.texture_ = 5;
    sort_key1.Decode(sort_key.Encode());

    bool ret1 = sort_key1.layer_ == sort_key.layer_  && \
    sort_key1.order_ == sort_key.order_ &&  \
    sort_key1.shader_ == sort_key.shader_ && \
    sort_key1.blend_ == sort_key.blend_ && \
    sort_key1.texture_ == sort_key.texture_ ;

    REQUIRE(ret1);
}

TEST_CASE("test_res_manager")
{
}

TEST_CASE("test_free_list")
{
    FreeList free_list =  FreeList();
    REQUIRE(free_list.Pop() == 0);

    free_list.Push(1);
    REQUIRE(free_list.Pop() == 1);

    free_list.Push(2);
    free_list.Push(3);
    free_list.Push(4);
    free_list.Push(5);

    REQUIRE(free_list.Pop() == 5);
}

TEST_CASE("test_res_Rect")
{
}

TEST_CASE("test_res_Render_Draw")
{
}

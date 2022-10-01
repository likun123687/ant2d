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
#include <gfx/bk/render_context.h>
#include <engi/entity.h>
#include <gfx/sprite/sprite.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform_table.h>
#include <math/vector.h>
#include <math/matrix.h>
#include <game/fps.h>

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
    sg_shader shid = sg_shader { 1 };
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
        shader.Create(kBatchShader);
        REQUIRE(shader.GetType() == kBatchShader);
        REQUIRE(shader.GetShdId().id == shid.id);
    }

    /*
    {
        REQUIRE_CALL(sokol_gfx_api_mock, batch_attr_slot(_))
            .IN_SEQUENCE(seq)
            .LR_WITH(std::string(_1) == std::string("aaa"))
            .RETURN(1);
        shader.AddAttributeBinding("aaa", 0, SG_VERTEXFORMAT_FLOAT3);
    }
    */

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

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_slot(_, _))
        .IN_SEQUENCE(seq)
        .LR_WITH(_1 == SG_SHADERSTAGE_FS && std::string(_2) == std::string("aaa"))
        .RETURN(1);

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_size(_, _))
        .IN_SEQUENCE(seq)
        .LR_WITH(_1 == SG_SHADERSTAGE_FS && std::string(_2) == std::string("aaa"))
        .RETURN(22);

    uniformblock.Create(kBatchShader, SG_SHADERSTAGE_FS, "aaa");
    REQUIRE(uniformblock.GetSlot() == 1);
    REQUIRE(uniformblock.GetSize() == 22);
    REQUIRE(uniformblock.GetStage() == SG_SHADERSTAGE_FS);
    REQUIRE(uniformblock.GetShaderType() == kBatchShader);

    uint32_t code = Uniformblock::Encode(1, 2, 3);
    uint8_t stage = 0;
    uint8_t slot = 0;
    uint8_t size = 0;
    Uniformblock::Decode(code, &stage, &slot, &size);
    REQUIRE((stage == 1 && slot == 2 && size == 3));

    UniformblockBuffer ubb = UniformblockBuffer();
    REQUIRE(ubb.IsEmpty());
    ubb.WriteUInt32(16);
    ubb.WriteUInt32(32);
    ubb.WriteUInt32(64);
    ubb.Seek(0);
    REQUIRE(16 == ubb.ReadUInt32());
    REQUIRE(32 == ubb.ReadUInt32());
    REQUIRE(64 == ubb.ReadUInt32());
    REQUIRE(12 == ubb.GetPos());

    ubb.Seek(0);
    void* data = ubb.ReadPointer(4);
    REQUIRE(*(static_cast<uint16_t*>(data)) == 16);

    ubb.Seek(0);
    uint8_t* data_ptr = new uint8_t[10];
    ubb.Copy(data_ptr, 10);
    delete[] data_ptr;
    REQUIRE(10 == ubb.GetPos());
}

TEST_CASE("test_sort_key")
{
    SortKey sort_key = SortKey();
    sort_key.layer_ = 1;
    sort_key.order_ = 2;
    sort_key.pipeline_ = 3;
    sort_key.blend_ = 4;
    sort_key.texture_ = 5;

    auto key = sort_key.Encode();
    REQUIRE(key == 0x10087005);

    auto sort_key1 = SortKey();
    sort_key1.Decode(key);

    bool ret = sort_key1.layer_ == 1
        && sort_key1.order_ == 2
        && sort_key1.pipeline_ == 3
        && sort_key1.blend_ == 4
        && sort_key1.texture_ == 5;

    REQUIRE(ret);

    sort_key.layer_ = 9;
    sort_key.order_ = 8;
    sort_key.pipeline_ = 7;
    sort_key.blend_ = 1;
    sort_key.texture_ = 5;
    sort_key1.Decode(sort_key.Encode());

    bool ret1 = sort_key1.layer_ == sort_key.layer_
        && sort_key1.order_ == sort_key.order_
        && sort_key1.pipeline_ == sort_key.pipeline_
        && sort_key1.blend_ == sort_key.blend_
        && sort_key1.texture_ == sort_key.texture_;

    REQUIRE(ret1);
}

TEST_CASE("test_res_manager")
{
    using trompeloeil::_; // wild card for matching any value
    trompeloeil::sequence seq;

    auto data = new uint8_t[64];
    sg_buffer buffer_id;
    buffer_id.id = 1;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_buffer(_))
        .WITH(_1->data.ptr == data && _1->data.size == 64)
        .IN_SEQUENCE(seq)
        .TIMES(AT_LEAST(1))
        .RETURN(buffer_id);

    uint16_t index_id = 0;
    IndexBuffer* ib = nullptr;
    std::tie(index_id, ib) = SharedResManager.AllocIndexBuffer(data, 64);
    IndexBuffer* ib1 = SharedResManager.GetIndexBuffer(index_id);
    REQUIRE(ib == ib1);

    std::tie(index_id, ib) = SharedResManager.AllocIndexBuffer(data, 64);
    REQUIRE(ib == SharedResManager.GetIndexBuffer(index_id));

    // test get shader
    auto shader = Shader();
    sg_shader shid = sg_shader { 1 };
    sg_shader_desc sh_desc = {};
    sh_desc.label = "batch_shader_unique";

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

    uint16_t sh_id = kInvalidId;
    Shader* sh = nullptr;
    std::tie(sh_id, sh) = SharedResManager.AllocShader(ShaderType::kBatchShader);
    Info("shader id {} sh {}", sh_id, static_cast<void*>(sh));
    REQUIRE(sh == SharedResManager.GetShader(sh_id));
}

TEST_CASE("test_free_list")
{
    FreeList free_list = FreeList();
    REQUIRE(free_list.Pop() == 0);

    free_list.Push(1);
    REQUIRE(free_list.Pop() == 1);

    free_list.Push(2);
    free_list.Push(3);
    free_list.Push(4);
    free_list.Push(5);

    REQUIRE(free_list.Pop() == 5);
}

TEST_CASE("test_render_context_rect")
{
    Rect rect = Rect();
    REQUIRE(rect.IsZero());
}

TEST_CASE("test_res_render_draw")
{
    RenderDraw render_draw = RenderDraw();
    render_draw.Reset();
}

TEST_CASE("test_render_context")
{
    ResManager res_manager = ResManager();
    UniformblockBuffer ubb = UniformblockBuffer();

    RenderContext ctx = RenderContext(&res_manager, &ubb);
    ctx.Reset();
}

TEST_CASE("test_entity")
{
    EntityManager em = EntityManager();
    Entity e = em.New();
    REQUIRE(em.Alive(e));
    em.Destroy(e);
    REQUIRE(!em.Alive(e));

    Entity e1 = em.New();
    REQUIRE(e.Index() == e1.Index());
    REQUIRE(e1.Gene() == (e.Gene() + 1));
}

TEST_CASE("test_comp")
{
    auto sprite_comp = SpriteComp();
    REQUIRE(sprite_comp.GetEntity().IsGhost());
}

TEST_CASE("test_sprite_table")
{
    auto sprite_table = SpriteTable();
    EntityManager em = EntityManager();

    Entity e = em.New();
    auto sprite_comp = sprite_table.NewComp(e);
    REQUIRE(sprite_comp->GetEntity().Index() == e.Index());

    Entity e1 = em.New();
    auto sprite_comp1 = sprite_table.NewComp(e1);
    REQUIRE(sprite_comp1->GetEntity().Index() == e1.Index());

    REQUIRE(sprite_table.GetComp(e) == sprite_comp);
    REQUIRE(sprite_table.GetComp(e1) == sprite_comp1);
    REQUIRE(sprite_table.GetComp(e1) != sprite_comp);

    uint32_t idx = sprite_table.GetCompIdx(e);
    uint32_t idx1 = sprite_table.GetCompIdx(e1);
    REQUIRE(idx != idx1);

    REQUIRE(sprite_table.GetComp(idx) == sprite_comp);
    REQUIRE(sprite_table.GetComp(idx1) == sprite_comp1);

    sprite_table.Delete(e);
    REQUIRE(sprite_table.GetComp(e) == nullptr);
    REQUIRE(sprite_table.GetSize() == 1);

    auto entity_list = std::vector<Entity>();
    auto sprite_comp_list = std::vector<SpriteComp*>();
    for (int i = 0; i < 1000; i++) {
        Entity entity = em.New();
        auto sprite_comp_new = sprite_table.NewComp(entity);
        entity_list.push_back(entity);
        sprite_comp_list.push_back(sprite_comp_new);
    }

    REQUIRE(sprite_table.GetSize() == 1001);
    REQUIRE(sprite_comp_list[50] == sprite_table.GetComp(entity_list[50]));

    sprite_table.Destroy();
    REQUIRE(sprite_table.GetComp(e1) == nullptr);
    REQUIRE(sprite_table.GetSize() == 0);
}

TEST_CASE("test_transform_table")
{
    auto transform_table = TransformTable();
    EntityManager em = EntityManager();

    Entity e = em.New();
    Entity e1 = em.New();
    Entity e2 = em.New();

    auto transform = transform_table.NewComp(e);
    transform->SetPosition(math::Vec2 { 100, 100 });
    auto pos = transform->GetPosition();
    REQUIRE((pos[0] == 100 && pos[1] == 100));
    REQUIRE(transform->GetLocal().position[0] == 100);
    REQUIRE(transform->GetWorld().position[0] == 100);

    auto transform1 = transform_table.NewComp(e1);
    auto transform2 = transform_table.NewComp(e2);

    transform->LinkChild(transform1);
    transform->LinkChild(transform2);

    transform1->SetPosition(math::Vec2 { 50, 50 });
    transform2->SetPosition(math::Vec2 { -50, -50 });

    auto pos1 = transform1->GetWorld().position;
    REQUIRE(pos1[0] == 150);

    auto pos2 = transform2->GetWorld().position;
    REQUIRE((pos2[0] == 50 && pos2[1] == 50));

    transform->SetPosition(math::Vec2 { 200, 200 });
    pos1 = transform1->GetWorld().position;
    REQUIRE((pos1[0] == 250 && pos1[1] == 250));

    pos2 = transform2->GetWorld().position;
    REQUIRE((pos2[0] == 150 && pos2[1] == 150));

    // test node link
    Entity car_e = em.New();
    auto car_comp = transform_table.NewComp(car_e);

    Entity wheel1_e = em.New();
    Entity wheel2_e = em.New();
    Entity wheel3_e = em.New();
    Entity wheel4_e = em.New();
    auto wheel1_comp = transform_table.NewComp(wheel1_e);
    auto wheel2_comp = transform_table.NewComp(wheel2_e);
    auto wheel3_comp = transform_table.NewComp(wheel3_e);
    auto wheel4_comp = transform_table.NewComp(wheel4_e);

    std::vector<Transform*> wheel_list = { wheel1_comp, wheel2_comp, wheel3_comp, wheel4_comp };
    car_comp->LinkChildren(wheel_list);
    for (auto& w : wheel_list) {
        auto parent_tran = w->Parent();
        REQUIRE(parent_tran->GetEntity() == car_e);
    }

    REQUIRE(car_comp->FirstChild()->GetEntity() == wheel1_e);

    for (int i = 0; i < 3; i++) {
        Transform* next_sibling = nullptr;
        std::tie(std::ignore, next_sibling) = wheel_list[i]->Sibling();
        REQUIRE(next_sibling->GetEntity() == wheel_list[i + 1]->GetEntity());
    }

    for (int i = 3; i > 0; i--) {
        Transform* prev_sibling = nullptr;
        std::tie(prev_sibling, std::ignore) = wheel_list[i]->Sibling();
        REQUIRE(prev_sibling->GetEntity() == wheel_list[i - 1]->GetEntity());
    }

    Transform* prev_sibling = nullptr;
    Transform* next_sibling = nullptr;
    std::tie(prev_sibling, next_sibling) = wheel1_comp->Sibling();
    REQUIRE(prev_sibling == nullptr);
    REQUIRE(next_sibling == wheel2_comp);
    // car_comp->dump();
    // Info("====================");
    // wheel2_comp->dump();
    // wheel4_comp->dump();
    // wheel1_comp->dump();
    // wheel3_comp->dump();
    // wheel1_sub_comp->dump();
    // wheel1_sub_comp1->dump();

    // REQUIRE(transform_table.GetSize() == 7);

    // transform_table.Delete(wheel1_e);
    ////transform_table.Delete(wheel3_e);

    // wheel1_comp = transform_table.GetComp(wheel1_e);
    // REQUIRE(wheel1_comp == nullptr);
    // wheel2_comp = transform_table.GetComp(wheel2_e);
    // wheel4_comp = transform_table.GetComp(wheel4_e);
    // wheel3_comp = transform_table.GetComp(wheel3_e);

    // REQUIRE(wheel2_comp->GetNxtSiblingIdx() == transform_table.GetCompIdx(wheel4_e));
    // REQUIRE(wheel4_comp->GetNxtSiblingIdx() == transform_table.GetCompIdx(wheel3_e));

    // Info("====================");

    // wheel2_comp->dump();
    // wheel4_comp->dump();
    // wheel3_comp->dump();

    ////wheel3_comp->dump();
    // transform_table.Delete(wheel3_e);
    // wheel2_comp = transform_table.GetComp(wheel2_e);
    // wheel4_comp = transform_table.GetComp(wheel4_e);
    // Info("====================");
    // wheel2_comp->dump();
    // wheel4_comp->dump();

    // REQUIRE(transform_table.GetComp(wheel1_e) == nullptr);
    // REQUIRE(transform_table.GetComp(wheel3_e) == nullptr);
    // REQUIRE(transform_table.GetSize() == 3);
}

/***
0
|
1-2-3
|
4 -5- 6-13-15
|     |
7     8-9-14
|     |
11-17 10-12
|  |  |
16 18 19
***/
TEST_CASE("test_transform_table_more")
{
    auto transform_table = TransformTable();
    EntityManager em = EntityManager();

    std::vector<Entity> entity_list;
    std::vector<Transform*> comp_list;
    for (int i = 0; i < 20; i++) {
        Entity e = em.New();
        auto comp = transform_table.NewComp(e);
        entity_list.push_back(e);
        comp_list.push_back(comp);
    }

    comp_list[0]->LinkChildren({ comp_list[1], comp_list[2], comp_list[3] });
    comp_list[1]->LinkChildren({ comp_list[4], comp_list[5], comp_list[6], comp_list[13], comp_list[15] });
    comp_list[4]->LinkChild(comp_list[7]);

    comp_list[7]->LinkChildren({ comp_list[11], comp_list[17] });

    comp_list[11]->LinkChild(comp_list[16]);
    comp_list[17]->LinkChild(comp_list[18]);

    comp_list[6]->LinkChildren({ comp_list[8], comp_list[9], comp_list[14] });
    comp_list[8]->LinkChildren({ comp_list[10], comp_list[12] });
    comp_list[12]->LinkChildren({ comp_list[19] });

    REQUIRE(transform_table.GetComp(entity_list[19]) == comp_list[19]);
    REQUIRE(transform_table.GetSize() == 20);

    REQUIRE(comp_list[8]->GetParentIdx() == transform_table.GetCompIdx(entity_list[6]));

    /////
    comp_list[0]->LinkChild(comp_list[11]);
    REQUIRE(comp_list[3]->GetNxtSiblingIdx() == transform_table.GetCompIdx(entity_list[11]));
    REQUIRE(comp_list[7]->GetFirstChildIdx() == transform_table.GetCompIdx(entity_list[17]));
    ////

    REQUIRE(comp_list[5]->GetNxtSiblingIdx() == transform_table.GetCompIdx(entity_list[6]));
    comp_list[15]->LinkChild(comp_list[6]);
    REQUIRE(comp_list[5]->GetNxtSiblingIdx() == transform_table.GetCompIdx(entity_list[13]));
    REQUIRE(comp_list[15]->GetFirstChildIdx() == transform_table.GetCompIdx(entity_list[6]));
}

TEST_CASE("test_transform_table_delete")
{
    auto transform_table = TransformTable();
    EntityManager em = EntityManager();

    std::vector<Entity> entity_list;
    std::vector<Transform*> comp_list;
    for (int i = 0; i < 20; i++) {
        Entity e = em.New();
        auto comp = transform_table.NewComp(e);
        entity_list.push_back(e);
        comp_list.push_back(comp);
    }

    comp_list[0]->LinkChildren({ comp_list[1], comp_list[2], comp_list[3] });
    comp_list[1]->LinkChildren({ comp_list[4], comp_list[5], comp_list[6], comp_list[13], comp_list[15] });
    comp_list[4]->LinkChild(comp_list[7]);

    comp_list[7]->LinkChildren({ comp_list[11], comp_list[17] });

    comp_list[11]->LinkChild(comp_list[16]);
    comp_list[17]->LinkChild(comp_list[18]);

    comp_list[6]->LinkChildren({ comp_list[8], comp_list[9], comp_list[14] });
    comp_list[8]->LinkChildren({ comp_list[10], comp_list[12] });
    comp_list[12]->LinkChildren({ comp_list[19] });

    REQUIRE(transform_table.GetComp(entity_list[19]) == comp_list[19]);
    REQUIRE(transform_table.GetSize() == 20);

    transform_table.Delete(entity_list[4]);
    REQUIRE(transform_table.GetSize() == 14);

    REQUIRE(comp_list[1]->GetFirstChildIdx() == transform_table.GetCompIdx(entity_list[5]));
    REQUIRE(transform_table.GetComp(entity_list[11]) == nullptr);
}

TEST_CASE("test_math")
{
    auto vec2_1 = math::Vec2 { 1, 1 };
    REQUIRE(vec2_1[0] == 1);
    REQUIRE(vec2_1[1] == 1);

    auto vec2_2 = math::Vec2 { 1, 1 };
    auto vec2_3 = vec2_1.Add(vec2_2);
    REQUIRE(vec2_3[0] == 2);
    REQUIRE(vec2_3[1] == 2);

    auto vec2_4 = math::Vec2 { 3.0, 4.0 };
    REQUIRE(vec2_4.Len() == 5);

    auto vec3 = math::Vec3 {};
    REQUIRE(vec3[0] == 0);
    REQUIRE(vec3[1] == 0);
    REQUIRE(vec3[2] == 0);

    auto vec3_1 = math::Vec3 { 1, 1, 1 };
    REQUIRE(vec3_1[0] == 1);
    REQUIRE(vec3_1[1] == 1);
    REQUIRE(vec3_1[2] == 1);
    auto vec3_2 = vec3.Add(vec3_1);

    REQUIRE(vec3_2[0] == 1);
    REQUIRE(vec3_2[1] == 1);
    REQUIRE(vec3_2[2] == 1);

    auto vec4_1 = math::Vec4 { 1, 1, 1, 1 };
    auto vec4_2 = math::Vec4 { 2, 2, 2, 2 };

    REQUIRE(vec4_1[0] == 1);
    REQUIRE(vec4_1[1] == 1);
    REQUIRE(vec4_1[2] == 1);
    REQUIRE(vec4_1[3] == 1);

    REQUIRE(vec4_2[0] == 2);
    REQUIRE(vec4_2[1] == 2);
    REQUIRE(vec4_2[2] == 2);
    REQUIRE(vec4_2[3] == 2);

    auto ident3 = math::Mat3::Ident3();
    auto diag = ident3.Diag();
    REQUIRE(diag[0] == 1);
    REQUIRE(diag[1] == 1);
    REQUIRE(diag[2] == 1);
}

TEST_CASE("test_fps")
{
    auto fps = FPS();
    fps.Init();
    // TODO
}

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

#include <gfx/batch_render.h>
#include <gfx/sprite/sprite_render_feature.h>
#include <asset/texture_manager.h>
#include <math/vector.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform_table.h>
#include <gfx/bk/bk.h>
#include <gfx/gctx.h>

using namespace ant2d;

TEST_CASE("test_batch_render")
{
    using trompeloeil::_; // wild card for matching any value
    trompeloeil::sequence seq;

    sg_context_desc ctx_desc = {};
    ctx_desc.sample_count = 5555;
    sg_desc desc = {};
    desc.context = ctx_desc;

    REQUIRE_CALL(sokol_gfx_api_mock, sapp_sgcontext())
        .IN_SEQUENCE(seq)
        .RETURN(ctx_desc);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_setup(_))
        .IN_SEQUENCE(seq)
        .WITH(_1->context.sample_count == 5555);

    gfx::Init(1);

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

    sg_pipeline pip = { 1 };

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_pipeline(_))
        .IN_SEQUENCE(seq)
        .RETURN(pip);

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_slot(_, _))
        .IN_SEQUENCE(seq)
        .RETURN(1);

    REQUIRE_CALL(sokol_gfx_api_mock, batch_uniformblock_size(_, _))
        .IN_SEQUENCE(seq)
        .RETURN(22);

    BatchRender batch_render { ShaderType::kBatchShader };
    batch_render.Begin(1, 1);

    auto sprite_table = SpriteTable();
    sprite_table.SetTableType(TableType::kSprite);
    auto transform_table = TransformTable();
    transform_table.SetTableType(TableType::kTransform);

    sg_image image_id;
    image_id.id = 1;
    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
        .IN_SEQUENCE(seq)
        .RETURN(image_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
        .IN_SEQUENCE(seq);

    sg_buffer buffer_id;
    buffer_id.id = 1;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_buffer(_))
        .TIMES(2)
        .RETURN(buffer_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_update_buffer(_, _))
        .IN_SEQUENCE(seq);

    auto texture_manager = TextureManager();
    texture_manager.Load("assets/face.png");
    auto tex = texture_manager.Get("assets/face.png");

    auto entity_manager = EntityManager();
    auto entity = entity_manager.New();

    auto sprite_comp = sprite_table.NewCompX(entity, tex);
    sprite_comp->SetSize(80, 80);
    auto xf = transform_table.NewComp(entity);
    xf->SetPosition(ant2d::math::Vec2 { 300, 300 });

    auto sprite_batch_object = SpriteBatchObject {};
    sprite_batch_object.SetSpriteComp(sprite_comp);
    sprite_batch_object.SetTransform(xf);
    auto context = batch_render.GetBatchContext();
    REQUIRE(context->GetVertexPos() == 0);

    batch_render.Draw(&sprite_batch_object);
    batch_render.Draw(&sprite_batch_object);

    auto tex_id = context->GetTexId();

    REQUIRE(tex_id == 1);
    REQUIRE(context->GetVertexPos() == 8);

    batch_render.End();
    auto batch_used = context->GetBatchUsed();
    auto batch_list = context->GetBatchList();
    REQUIRE(batch_used == 1);
    REQUIRE(batch_list[batch_used - 1].texture_id == 1);
    REQUIRE(batch_list[batch_used - 1].num_vertex == 8);
    REQUIRE(batch_list[batch_used - 1].first_index == 0);
    REQUIRE(batch_list[batch_used - 1].num_index == 12);

    batch_render.Begin(2, 2);
    batch_render.Draw(&sprite_batch_object);
    batch_render.Draw(&sprite_batch_object);
    batch_render.Draw(&sprite_batch_object);
    batch_render.End();
    batch_used = context->GetBatchUsed();
    auto batch_list1 = context->GetBatchList();

    REQUIRE(batch_used == 2);
    REQUIRE(batch_list1[batch_used - 1].texture_id == 2);
    REQUIRE(batch_list1[batch_used - 1].num_vertex == 12);
    REQUIRE(batch_list1[batch_used - 1].first_index == 12);
    REQUIRE(batch_list1[batch_used - 1].num_index == 18);

    REQUIRE(batch_render.Flush() == 2);

    REQUIRE(context->GetBatchUsed() == 0);
    REQUIRE(context->GetVertexPos() == 0);
}

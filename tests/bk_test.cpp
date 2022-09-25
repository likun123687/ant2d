#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>
#include <utils/content.h>
#include <gfx/bk/bk.h>
#include <asset/image_data.h>
#include <gfx/bk/res_manager.h>
#include <utils/debug.h>
#include <gfx/bk/queue.h>

using namespace ant2d;

TEST_CASE("test_init")
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

    bk::Init();
}

TEST_CASE("test_one_drawcall")
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
    bk::Init(); //先初始化

    // create texture
    auto file_content = SharedContent.LoadFile("assets/face.png");
    auto image_data = ImageData(file_content.first.get(), file_content.second);
    sg_image image_id;
    image_id.id = 1;
    REQUIRE_CALL(sokol_gfx_api_mock, sg_alloc_image())
        .IN_SEQUENCE(seq)
        .RETURN(image_id);

    REQUIRE_CALL(sokol_gfx_api_mock, sg_init_image(_, _))
        .LR_WITH((_1.id == image_id.id && _2->width == image_data.width_))
        .LR_WITH(_2->data.subimage[0][0].size == image_data.width_ * image_data.height_ * 4)
        .IN_SEQUENCE(seq);

    auto [tex_id, tex] = SharedResManager.AllocTexture(image_data);

    //创建vertex id
    auto vertex_data = new uint8_t[64];
    sg_buffer vertex_buffer;
    vertex_buffer.id = 1;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_buffer(_))
        .WITH(_1->data.ptr == vertex_data && _1->data.size == 64 && _1->usage == SG_USAGE_IMMUTABLE)
        .IN_SEQUENCE(seq)
        .TIMES(AT_LEAST(1))
        .RETURN(vertex_buffer);

    auto [vertex_id, vb] = SharedResManager.AllocVertexBuffer(vertex_data, 64, 1);

    //创建index id
    auto index_data = new uint8_t[64];
    sg_buffer index_buffer;
    index_buffer.id = 2;

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_buffer(_))
        .WITH(_1->data.ptr == index_data && _1->data.size == 64)
        .IN_SEQUENCE(seq)
        .TIMES(AT_LEAST(1))
        .RETURN(index_buffer);

    auto [index_id, ib] = SharedResManager.AllocIndexBuffer(index_data, 64);

    //创建shader
    sg_shader shid = sg_shader{1};
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

    auto [sh_id, sh] = SharedResManager.AllocShader(kBatchShader);

    //创建pipeline
    sg_pipeline_desc pdesc = {};
    pdesc.shader = sh->GetShdId();
    pdesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT4;
    pdesc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;
    pdesc.index_type = SG_INDEXTYPE_UINT16;

    sg_pipeline pip = {1};

    Info("pipe1 desc {}", static_cast<void *>(&pdesc));

    REQUIRE_CALL(sokol_gfx_api_mock, sg_make_pipeline(_))
        .IN_SEQUENCE(seq)
        .LR_WITH(_1 == &pdesc)
        .RETURN(pip);

    auto [pipeline_id, pipeline] = SharedResManager.AllocPipeline(&pdesc);

    bk::SetTexture(0, tex_id);
    bk::SetVertexBuffer(0, vertex_id);
    bk::SetIndexBuffer(index_id, 0, 6);
    auto draw_call = SharedRenderQueue.GetDrawCall();
    RenderDraw draw_call_copy = draw_call;

    REQUIRE(draw_call.num_ == 6);
    REQUIRE(draw_call.index_buffer_ == ResManager::TripType(index_id));
    REQUIRE(draw_call.vertex_buffers_[0] == ResManager::TripType(vertex_id));
    bk::Submit(0, pipeline_id, 0);

    auto &sort_keys = SharedRenderQueue.GetSortKeys();
    auto &sort_values = SharedRenderQueue.GetSortValues();
    auto &draw_call_list = SharedRenderQueue.GetDrawCallList();
    REQUIRE(sort_values[0] == 0);
    REQUIRE(draw_call_list[0].num_ == 6);
    REQUIRE(draw_call_list[0].vertex_buffers_[0] == ResManager::TripType(vertex_id));


    bk::SetTexture(0, tex_id);
    bk::SetVertexBuffer(0, vertex_id);
    bk::SetIndexBuffer(index_id, 6, 12);
    bk::Submit(0, pipeline_id, 0);

    REQUIRE(sort_values[1] == 1);
    REQUIRE(draw_call_list[1].num_ == 6);
    REQUIRE(draw_call_list[1].vertex_buffers_[0] == ResManager::TripType(vertex_id));

    //两个drawcall一样的
    bk::Flush();

    //两个drawcall不一样的
    //是否排序
}

TEST_CASE("test_multiple_drawcall")
{

}

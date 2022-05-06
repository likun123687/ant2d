#include <gfx/bk/buffer.h>
#include <cinttypes>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>

TEST_CASE("testBuffer")
{
    using trompeloeil::_;  // wild card for matching any value

    uint32_t size = 64;
    auto buffer = new uint8_t[static_cast<size_t>(size)];
    auto index_buffer = bk::IndexBuffer();

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


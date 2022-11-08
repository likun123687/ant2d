#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <tests/mocks/sokol_api.h>
#include <effect/chan.h>
#include <vector>
#include <effect/pool.h>

using namespace ant2d;

TEST_CASE("test_channel_f32")
{
    std::vector<float> store(1024);
    auto p = Channel_f32 { store.data(), 1024 };
    int32_t n = 10;

    p.Add(n, 8);

    for (int i = 0; i < n; i++) {
        REQUIRE(p[i] == 8);
    }

    p.Sub(n, 4);

    for (int i = 0; i < n; i++) {
        REQUIRE(p[i] == 4);
    }

    p.Mul(n, 2);
    for (int i = 0; i < n; i++) {
        REQUIRE(p[i] == 8);
    }

    std::vector<float> store1(1024);
    auto v = Channel_f32 { store1.data(), 1024 };
    v.SetConst(n, 1.0 / 60);

    p.Integrate(n, v, 2);

    float pp = float(8 + 1.0 / 60 * 2);
    for (int i = 0; i < n; i++) {
        REQUIRE(p[i] == pp);
    }
}

TEST_CASE("test_channel_v2")
{
    std::vector<math::Vec2> store(1024);
    auto p = Channel_v2 { store.data(), 1024 };
    int32_t n = 10;
    p.SetConst(n, 4, 8);
    for (int i = 0; i < n; i++) {
        REQUIRE((p[i][0] == 4 && p[i][1] == 8));
    }

    p.Add(n, 4, 8);
    for (int i = 0; i < n; i++) {
        REQUIRE((p[i][0] == 8 && p[i][1] == 16));
    }

    std::vector<math::Vec2> store1(1024);
    auto v = Channel_v2 { store1.data(), 1024 };
    v.SetConst(n, 16, 32);
    p.Integrate(n, v, 1.0 / 60);

    auto p1 = float(8 + 16 * 1.0 / 60);
    auto p2 = float(16 + 32 * 1.0 / 60);

    for (int i = 0; i < n; i++) {
        REQUIRE((p[i][0] == p1 || p[i][1] == p2));
    }
}

TEST_CASE("test_pool")
{
    auto p = Pool { 1024 };
    p.AddChan({ LIFE_CONST, POSITION_CONST });
    p.Initialize();

    auto size = 1024 * 4 + 1024 * 8;
    REQUIRE(p.Size() == size);

    auto life = std::get<1>(p.GetField(LIFE_CONST));
    auto pose = std::get<2>(p.GetField(POSITION_CONST));

    REQUIRE(life.GetSize() == 1024);
    REQUIRE(pose.GetSize() == 1024);

    int d = (uint8_t*)(pose.GetData()) - (uint8_t*)(life.GetData());
    REQUIRE(d == 1024 * 4);
}
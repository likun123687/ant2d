#pragma once
#include <cstdint>
namespace ant2d {
struct RefCount {
    void* ref;
    uint32_t cnt;
};

struct IdCount {
    uint16_t rid;
    uint16_t cnt;
};
}

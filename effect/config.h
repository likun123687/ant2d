#pragma once
#include <effect/common.h>

namespace ant2d {
struct CommonConfig {
    int max;
    float duration;
    float rate; // Number of particles per-second

    Var life;
    Var x;
    Var y;
    Range size;
    Range rot;

    Range r;
    Range g;
    Range b;
    Range a;
    bool additive;
};

} // namespace ant2d

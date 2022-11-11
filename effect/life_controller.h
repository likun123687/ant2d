#pragma once
#include <effect/chan.h>
#include <effect/pool.h>

namespace ant2d {

class LifeController {
public:
    LifeController()
        : life_ { nullptr, 0 }
        , live_ { 0 }
    {
    }
    Channel_f32 life_;
    int live_;

public:
    int GC(Pool* p);
};
} // namespace ant2d

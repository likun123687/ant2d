#pragma once
#include <engi/base_table.h>
#include <anim/frame/flipbook_comp.h>

namespace ant2d {
namespace frame {
    class FlipbookTable : public BaseTable<FlipbookComp> {
    public:
        FlipbookComp* NewComp(Entity entity);
    };
}
}
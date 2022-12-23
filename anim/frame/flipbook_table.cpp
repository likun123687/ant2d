#include <anim/frame/flipbook_table.h>

namespace ant2d {
namespace frame {
    FlipbookComp* FlipbookTable::NewComp(Entity entity)
    {
        FlipbookComp* comp = BaseTable::NewComp(entity);
        return comp;
    }
}
}
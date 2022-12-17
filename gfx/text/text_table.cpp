#include <gfx/text/text_table.h>

namespace ant2d {
TextComp* TextTable::NewComp(Entity entity)
{
    TextComp* comp = BaseTable::NewComp(entity);
    comp->SetColor(0xFFFFFFFF);
    comp->SetGravity(0.5, 0.5);
    comp->SetVisible(true);
    return comp;
}
}
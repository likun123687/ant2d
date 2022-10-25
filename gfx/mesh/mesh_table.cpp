#include <gfx/mesh/mesh_table.h>

namespace ant2d {
MeshComp* MeshTable::NewComp(Entity entity)
{
    MeshComp* comp = BaseTable::NewComp(entity);
    comp->SetSize(64, 64);
    comp->SetVisible(true);
    return comp;
}
}
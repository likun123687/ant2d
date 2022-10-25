#pragma once
#include <engi/base_table.h>
#include <gfx/mesh/mesh_comp.h>

namespace ant2d {
class MeshTable : public BaseTable<MeshComp> {
public:
    MeshComp* NewComp(Entity entity);
};
}

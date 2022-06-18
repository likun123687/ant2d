#pragma once
#include <engi/base_table.h>
#include <gfx/transform/transform.h>

namespace ant2d {
class TransformTable:public BaseTable<Transform>
{
public:
    Transform* NewComp(Entity entity);
    void Delete(Entity entity);
    //void Relink(uint16_t old_idx, uint16_t new_idx);
    void TailDelete(uint16_t to_delete_idx);
};
}

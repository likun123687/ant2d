#pragma once
#include <engi/base_table.h>
#include <gfx/sprite/sprite.h>

namespace ant2d {
class SpriteTable:public BaseTable<SpriteComp>
{
public:
    SpriteComp* NewComp(Entity entity);
    SpriteComp *NewCompX(Entity entity, ITexture2D *spt);
};
}

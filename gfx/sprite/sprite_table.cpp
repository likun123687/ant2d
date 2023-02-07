#include <gfx/sprite/sprite.h>
#include <gfx/sprite/sprite_table.h>
#include <utils/debug.h>

namespace ant2d {
SpriteComp* SpriteTable::NewComp(Entity entity)
{
    SpriteComp *comp = BaseTable::NewComp(entity);
    comp->SetGravity(0.5, 0.5);
    comp->SetColor({0xFF, 0xFF, 0xFF, 0xFF});
    comp->SetVisible(true);
    return comp;
}

SpriteComp * SpriteTable::NewCompX(Entity entity, ITexture2D *spt)
{
    SpriteComp *comp = NewComp(entity);
    comp->SetSprite(spt);
    Info("new comp ptr {}", static_cast<void *>(comp));
    return comp;
}
}

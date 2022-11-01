#include <effect/particle_table.h>
#include <utils/debug.h>

namespace ant2d {
ParticleComp* ParticleTable::NewComp(Entity entity)
{
    ParticleComp* comp = BaseTable::NewComp(entity);
    comp->SetVisible(true);
    comp->SetSize(64, 64);

    return comp;
}
}

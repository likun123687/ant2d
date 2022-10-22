#pragma once
#include <engi/base_table.h>
#include <effect/particle.h>

namespace ant2d {
class ParticleTable : public BaseTable<ParticleComp> {
public:
    ParticleComp* NewComp(Entity entity);
};
}

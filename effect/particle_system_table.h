#pragma once
#include <engi/base_table.h>
#include <effect/particle.h>

namespace ant2d {
class ParticleSystemTable : public BaseTable<ParticleComp> {
public:
    ParticleComp* NewComp(Entity entity);
};
}

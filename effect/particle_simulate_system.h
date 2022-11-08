#pragma once
#include <effect/particle_system_table.h>

namespace ant2d {
class ParticleSimulateSystem {
private:
    ParticleSystemTable* pst_;

public:
    void RequireTable(TableList* tables);

    void Init();

    // TODO:
    // Need a better way to initialize each simulator
    void Update(float dt);
};
} // namespace ant2d

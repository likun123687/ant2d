#include <effect/particle_system_table.h>
#include <effect/particle_simulate_system.h>

namespace ant2d {

void ParticleSimulateSystem::RequireTable(TableList* tables)
{
    for (auto& t : (*tables)) {
        if (t->GetTableType() == TableType::kParticle) {
            pst_ = reinterpret_cast<ParticleSystemTable*>(t.get());
        }
    }
}

void ParticleSimulateSystem::Init()
{
}

// TODO:
// Need a better way to initialize each simulator
void ParticleSimulateSystem::Update(float dt)
{
    // initialize
    auto et = pst_;
    for (int i = 0, n = et->GetSize(); i < n; i++) {
        auto comp = et->GetComp(i);
        if (!comp->GetIsInit()) {
            comp->SetIsInit(true);
            comp->Init();
        }
    }
    // simulate
    for (int i = 0, n = et->GetSize(); i < n; i++) {
        auto comp = et->GetComp(i);
        comp->GetSimulator()->Simulate(dt);
    }
}
};

#include <game/script/script_system.h>

namespace ant2d {

void ScriptSystem::RequireTable(TableList* tables)
{
    for (auto& table : *tables) {
        if (table->GetTableType() == TableType::kScript) {
            script_table_ = reinterpret_cast<ScriptTable*>(table.get());
            break;
        }
    }
}

void ScriptSystem::Update(float dt)
{
    for (int i = 0; i < script_table_->GetSize(); i++) {
        ScriptComp* comp = script_table_->GetComp(i);
        auto script = comp->GetScript();
        if (script) {
            script->Update(dt);
        }
    }
}
} // namespace ant2d

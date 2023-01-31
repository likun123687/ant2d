#pragma once
#include <engi/base_table.h>
#include <game/script/script_comp.h>
#include <game/script/script.h>

namespace ant2d {
class ScriptTable : public BaseTable<ScriptComp> {
public:
    ScriptComp* NewComp(Entity entity, Script* script)
    {
        ScriptComp* comp = BaseTable::NewComp(entity);
        comp->SetScript(script);
        return comp;
    }
};
}

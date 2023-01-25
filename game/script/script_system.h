#pragma once
#include <game/script/script_table.h>

namespace ant2d {
class ScriptSystem {
private:
    ScriptTable* script_table_;

public:
    void RequireTable(TableList* tables);
    void Update(float dt);
};
} // namespace ant2d

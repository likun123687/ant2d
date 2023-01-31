#pragma once
#include <gfx/i_comp.h>
#include <gfx/render_data.h>
#include <game/script/script.h>

namespace ant2d {
class ScriptComp : public IComp {
private:
    std::unique_ptr<Script> script_;

public:
    void SetScript(Script* script)
    {
        script_.reset(script);
    }

    void Reset()
    {
        script_.reset();
    }

    Script* GetScript()
    {
        return script_.get();
    }
};
}
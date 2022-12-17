#pragma once
#include <engi/base_table.h>
#include <gfx/text/text_comp.h>

namespace ant2d {
class TextTable : public BaseTable<TextComp> {
public:
    TextComp* NewComp(Entity entity);
};
}

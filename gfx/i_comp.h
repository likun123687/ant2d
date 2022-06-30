#pragma once
#include <engi/entity.h>

namespace ant2d {
class IComp {
protected:
    Entity entity_;
public:
    IComp():entity_(Ghost)
    {
    }

    IComp(Entity e):entity_(e)
    {
    }
    void SetEntity(Entity entity)
    {
        entity_ = entity;
    }

    Entity GetEntity()
    {
        return entity_;
    }

    virtual void Reset() = 0;
    virtual ~IComp() = default;
};
}

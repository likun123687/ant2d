#pragma once
namespace ant2d {
struct Script {
    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Destroy() = 0;
    virtual ~Script() = default;
};
} // namespace ant2d

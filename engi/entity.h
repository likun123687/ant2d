#pragma once
#include <vector>
#include <deque>

namespace ant2d {

constexpr int32_t kIndexBits = 24;
constexpr int32_t kIndexMask = (1 << kIndexBits) - 1;
constexpr uint8_t kGenerationBits = 8;
constexpr uint8_t kGenerationMask = (1 << kGenerationBits) - 1;

class Entity {
public:
    Entity()
        : idx_(0xFFFFFFFF)
    {
    }
    Entity(uint32_t idx)
    {
        idx_ = idx;
    }
    uint32_t Index()
    {
        return idx_ & kIndexMask;
    }

    uint8_t Gene()
    {
        return (idx_ >> kIndexBits) & kGenerationMask;
    }

    bool IsGhost()
    {
        return idx_ == 0xFFFFFFFF;
    }
    friend bool operator==(const Entity& e1, const Entity& e2);
    friend bool operator!=(const Entity& e1, const Entity& e2);

private:
    uint32_t idx_;
};

const Entity Ghost = Entity(0xFFFFFFFF);

class EntityManager {
public:
    Entity New();
    bool Alive(Entity e);
    void Destroy(Entity e);

private:
    std::vector<uint8_t> generation_;
    std::deque<uint32_t> freelist_;
};

} // namespace ant2d

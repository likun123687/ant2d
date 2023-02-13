#include <engi/entity.h>

namespace ant2d {

const Entity Ghost { 0xFFFFFFFF };
Entity EntityManager::New()
{
    uint32_t ei = 0;
    uint8_t eg = 0;

    if (!freelist_.empty()) {
        ei = freelist_.front();
        freelist_.pop_front();
        eg = generation_[ei];
    } else {
        ei = static_cast<uint32_t>(generation_.size());
        generation_.push_back(0);
    }
    return Entity((static_cast<uint32_t>(eg) << kIndexBits) | ei);
}

bool EntityManager::Alive(Entity e)
{
    return generation_[e.Index()] == e.Gene();
}

void EntityManager::Destroy(Entity e)
{
    auto ei = e.Index();
    generation_[ei]++;
    freelist_.push_back(ei);
}

bool operator==(const Entity& e1, const Entity& e2)
{
    return e1.idx_ == e2.idx_;
}

bool operator!=(const Entity& e1, const Entity& e2)
{
    return e1.idx_ != e2.idx_;
}

}

namespace engi {

const int32_t IndexBits = 24;
const int32_t IndexMask = (1<< IndexBits)-1;
const uint8_t GenerationBits = 8;
const uint8_t GenerationMask = (1<< GenerationBits)-1;

class Entity {
public:
    Entity(uint32_t idx) {
        idx_ = idx;
    }
   uint32 Index() {
    return this->idx_ & IndexMask;
   }

    uint8 Gene()  {
        return (idx_ >>IndexBits) & GenerationMask
    }
private:
    uint32_t idx_;
}

const Entity Ghost = Entity(0xFFFFFFFF);


class EntityManager {
    private:
    std::vector<uint8_t> generation_;
    std::deque<uint32_t> freelist_;
    Entity id_;
};

Entity EntityManager::New() {
    uint32_t ei;
    uint8_t eg;

    if (!freelist_.empty()) {
        ei = freelist_.front();freelist_.pop_front();
        eg = generation_[ei];
    } else {
        ei = static_cast<uint32_t>(generation_.size());
        generation_.push_back(0);
    }
    return Entity((static_cast<uint32_t>(eg) << IndexBits) | ei);
}

bool EntityManager::Alive(Entity e)
{
    return generation_[e.Index()] == e.Gene()
}

void EntityManager::Destroy(Entity e)
{
    auto ei = e.Index();
    generation_[ei] ++;
    freelist_.push_back(ei);
}

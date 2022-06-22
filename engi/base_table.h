#pragma once

#include <unordered_map>
#include <vector>
#include <engi/entity.h>
#include <memory>

namespace ant2d {
enum class TableType
{
    kSprite,
    kTransform,
};

class IBase
{
public:
    virtual TableType GetTableType() = 0;
    virtual void SetTableType(TableType table_type) = 0;
    virtual ~IBase() = default;
};

static const int kStep = 64;
static const uint16_t kInvalidIdx = 0xFFFF;

template<typename T>
class BaseTable:public IBase
{
public:
    BaseTable():comps_(), map_(), index_(0)
    {
    }

    virtual T* NewComp(Entity entity)
    {
        //resize
        if (index_>=comps_.size()) {
            comps_.reserve(comps_.size()+kStep);
            for (int i=0; i<kStep; i++) {
              comps_.emplace_back(new T());
            }
        }

        auto ei = entity.Index();
        auto iter = map_.find(ei);
        if (iter != map_.end()) {
            return comps_[iter->second].get();
        }

        auto &comp = comps_[index_];
        comp->SetEntity(entity);
        map_[ei] = index_;
        index_++;
        return comp.get();
    }

    //bool IsAlive(Entity entity)
    //{
    //    auto ei = entity.Index();
    //    auto iter = map_.find(ei);
    //    if (iter != map_.end()) {
    //        return comps_[iter->second]->GetEntity() != 0;
    //    }
    //    return false;
    //}

    T *GetComp(Entity entity)
    {
        auto ei = entity.Index();
        auto iter = map_.find(ei);
        if (iter != map_.end()) {
            return comps_[iter->second].get();
        }
        return nullptr;
    }

    T *GetComp(uint16_t idx)
    {
        if (idx >= index_) {
            return nullptr;
        }

        return comps_[idx].get();
    }

    uint32_t GetCompIdx(Entity entity)
    {
        auto ei = entity.Index();
        auto iter = map_.find(ei);
        if (iter != map_.end()) {
            return iter->second;
        }
        return kInvalidIdx;
    }

    virtual void Delete(Entity entity)
    {
        auto ei = entity.Index();
        auto iter = map_.find(ei);
        if (iter != map_.end()) {
            auto v = iter->second;
            int tail = index_ - 1;
            if (v != tail && tail > 0) {
                // remap index
                auto idx = comps_[tail]->GetEntity().Index();
                map_[idx] = v;
                std::swap(comps_[v], comps_[tail]);
                comps_[tail]->Reset();
            } else {
                comps_[tail]->Reset();
            }

            index_ -=1;
            map_.erase(iter);
        }
    }

    int GetSize() const
    {
        return index_;
    }

    void Destroy()
    {
        comps_.clear();
        map_.clear();
        index_ = 0;
    }

    TableType GetTableType()
    {
        return table_type_;
    }

    void SetTableType(TableType table_type)
    {
        table_type_ = table_type;
    }

protected:
    std::vector<std::unique_ptr<T>> comps_;
    std::unordered_map<uint32_t, int> map_;
    int index_; 
    TableType table_type_;
};

using TableList = std::vector<std::unique_ptr<IBase>>;
} //namespace ant2d

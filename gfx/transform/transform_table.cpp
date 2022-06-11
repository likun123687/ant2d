#include <gfx/transform/transform_table.h>

namespace ant2d {

Transform* TransformTable::NewComp(Entity entity)
{
    Transform *comp = BaseTable::NewComp(entity);
    comp->SetLocal(SRT{math::Vec2(1,1), 0.0f, math::Vec2(0,0)});
    comp->SetWorld(SRT{math::Vec2(1,1), 0.0f, math::Vec2(0,0)});
    comp->SetTransformTable(this);
    return comp;
}

void TransformTable::Delete(Entity entity)
{
    auto ei = entity.Index();
    auto iter = map_.find(ei);
    if (iter != map_.end()) {
        auto v = iter->second;
        int tail = index_ - 1;
        if (v != tail && tail > 0) {
            Relink(tail, v);
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

void TransformTable::Relink(uint16_t old_idx, uint16_t new_idx)
{
    auto xf = GetComp(old_idx);
    // relink parent
    auto parent_idx = xf->GetParentIdx();
    if (parent_idx != 0) {
        auto pxf = GetComp(parent_idx);
        if (pxf->GetFirstChildIdx() == old_idx) {
            pxf->SetFirstChildIdx(new_idx);
        } else {
            auto prev = GetComp(xf->GetPreSiblingIdx());
            prev->SetNxtSiblingIdx(new_idx);
        }
    }

    // relink children
    auto child = xf->GetFirstChildIdx();
    if (child != 0) {
        auto node = GetComp(child);
        node->SetParentIdx(new_idx);
    }
}
}

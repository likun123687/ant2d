#include <gfx/transform/transform_table.h>
#include <algorithm>
//#include <functional>

namespace ant2d {

Transform* TransformTable::NewComp(Entity entity)
{
    Transform* comp = BaseTable::NewComp(entity);
    comp->SetLocal(SRT { math::Vec2(1, 1), 0.0f, math::Vec2(0, 0) });
    comp->SetWorld(SRT { math::Vec2(1, 1), 0.0f, math::Vec2(0, 0) });
    comp->SetTransformTable(this);
    return comp;
}

void TransformTable::TailDelete(uint16_t to_delete_idx)
{
    int tail = index_ - 1;
    if (tail < 0) {
        return;
    }

    if (to_delete_idx != tail && tail > 0) {
        auto old_e_idx = comps_[tail]->GetEntity().Index();
        map_[old_e_idx] = to_delete_idx;

        auto new_e_idx = comps_[to_delete_idx]->GetEntity().Index();
        map_.erase(new_e_idx);

        std::swap(comps_[to_delete_idx], comps_[tail]);
        comps_[to_delete_idx]->Relink(tail, to_delete_idx);

        comps_[tail]->Reset();
    } else {
        auto new_e_idx = comps_[tail]->GetEntity().Index();
        map_.erase(new_e_idx);
        comps_[tail]->Reset();
    }

    index_ -= 1;
}

void TransformTable::Delete(Entity entity)
{
    Transform* to_delete_comp = GetComp(entity);
    if (to_delete_comp == nullptr) {
        return;
    }
    int to_delete_idx = GetCompIdx(entity);
    std::vector<int> to_delete_idx_list = { to_delete_idx };
    to_delete_comp->CollectSubCompIdx(to_delete_idx_list);
    std::sort(to_delete_idx_list.begin(), to_delete_idx_list.end(), std::greater<int>());
    comps_[to_delete_idx]->BreakLink();
    comps_[to_delete_idx]->ResetIdx();
    for (auto idx : to_delete_idx_list) {
        TailDelete(idx);
    }
}

// void TransformTable::Relink(uint16_t old_idx, uint16_t new_idx)
//{
//     auto xf = GetComp(old_idx);
//     // relink parent
//     auto parent_idx = xf->GetParentIdx();
//     if (parent_idx != kInvalidIdx) {
//         auto pxf = GetComp(parent_idx);
//         if (pxf->GetFirstChildIdx() == old_idx) {
//             pxf->SetFirstChildIdx(new_idx);
//         } else {
//             auto prev = GetComp(xf->GetPreSiblingIdx());
//             prev->SetNxtSiblingIdx(new_idx);
//         }
//     }
//
//     // relink children
//     auto child = xf->GetFirstChildIdx();
//     if (child != kInvalidIdx) {
//         auto node = GetComp(child);
//         node->SetParentIdx(new_idx);
//     }
// }
}

#include <gfx/transform/transform.h>
#include <gfx/transform/transform_table.h>
#include <utils/debug.h>

namespace ant2d {

Transform::Transform():world_{},local_{},
    parent_{kInvalidIdx}, first_child_{kInvalidIdx}, pre_sibling_{kInvalidIdx}, nxt_sibling_{kInvalidIdx},transform_table_(nullptr)
{
}

uint16_t Transform::GetParentIdx()
{
    return parent_;
}

void Transform::SetParentIdx(uint16_t idx)
{
    parent_ = idx;
}

uint16_t Transform::GetFirstChildIdx()
{
    return first_child_;
}

void Transform::SetFirstChildIdx(uint16_t idx)
{
    first_child_ = idx;
}

uint16_t Transform::GetPreSiblingIdx()
{
    return pre_sibling_;
}

void Transform::SetPreSiblingIdx(uint16_t idx)
{
    pre_sibling_ = idx;
}

uint16_t Transform::GetNxtSiblingIdx()
{
    return nxt_sibling_;
}

void Transform::SetNxtSiblingIdx(uint16_t idx)
{
    nxt_sibling_ = idx;
}

math::Vec2 Transform::GetPosition()
{
    return local_.position;
}

math::Vec2 Transform::GetScale()
{
    return local_.scale;
}

float Transform::GetRotation()
{
    return local_.rotation;
}

SRT Transform::GetLocal()
{
    return local_;
}

SRT Transform::GetWorld()
{
    return world_;
}

void Transform::SetLocal(SRT local)
{
    local_ = local;
}

void Transform::SetWorld(SRT world)
{
    world_ = world;
}

void Transform::SetPosition(math::Vec2 position)
{
    local_.position = position;
    if (parent_ == kInvalidIdx) {
        SetPosition(nullptr, position);
    } else {
        auto world = transform_table_->GetComp(parent_)->GetWorld();
        SetPosition(&world, position);
    }
}

void Transform::MoveBy(float dx, float dy)
{
    auto p = local_.position;
    p[0] = p[0]+dx;
    p[1]= p[1]+dy;
    SetPosition(p);
}

// update world location: world = parent.world + self.local
void Transform::SetPosition(const SRT *parent, math::Vec2 local)
{
    auto p = math::Vec2(0, 0);
    if (parent) {
        p = parent->position;
    }

    world_.position[0] = p[0] + local[0];
    world_.position[1] = p[1] + local[1];

    // all child
    auto child = first_child_;
    while (child != kInvalidIdx) {
        auto node = transform_table_->GetComp(child);
        node->SetPosition(&world_, node->GetLocal().position);
        child = node->GetNxtSiblingIdx();
    }
}

void Transform::SetScale(math::Vec2 scale)
{
    local_.scale = scale;
    if (parent_ == kInvalidIdx) {
        SetScale(nullptr, scale);
    } else {
        auto world = transform_table_->GetComp(parent_)->GetWorld();
        SetScale(&world, scale);
    }
}

void Transform::SetScale(const SRT *parent, math::Vec2 scale)
{
    auto s = math::Vec2(1, 1);
    if (parent) {
        s = parent->scale;
    }
    world_.scale[0] = s[0] * scale[0];
    world_.scale[1] = s[1] * scale[1];

    auto child = first_child_;
    while (child != kInvalidIdx) {
        auto node = transform_table_->GetComp(child);
        node->SetScale(&world_, node->GetLocal().scale);
        child = node->GetNxtSiblingIdx();
    }
}

void Transform::ScaleBy(float dx, float dy)
{
    auto sk = local_.scale;
    sk[0] = sk[0]+dx;
    sk[1] = sk[1]+dy;
    SetScale(sk);
}

void Transform::SetRotation(float rotation)
{
    local_.rotation = rotation;
    // compute world rotation
    if (parent_ == kInvalidIdx) {
        SetRotation(nullptr, rotation);
    } else {
        auto world = transform_table_->GetComp(parent_)->GetWorld();
        SetRotation(&world, rotation);
    }
}

void Transform::SetRotation(const SRT *parent, float rotation)
{
    float r = 0.0f;
    if (parent != nullptr) {
        r = parent->rotation;
    }
    world_.rotation = r + rotation;

    auto child = first_child_;
    while (child != kInvalidIdx) {
        auto node = transform_table_->GetComp(child);
        node->SetRotation(&world_, node->GetLocal().rotation);
        child = node->GetNxtSiblingIdx();
    }
}

void Transform::RotateBy(float d)
{
    auto r = local_.rotation;
    r+= d;
    SetRotation(r);
}

void Transform::LinkChildren(std::vector<Transform *> child_list)
{
    for (auto c : child_list) {
        LinkChild(c);
    }
}

void Transform::LinkChild(Transform *c)
{
    auto pi = transform_table_->GetCompIdx(entity_);
    auto ci = transform_table_->GetCompIdx(c->GetEntity());

    //已经是了
    if (c->GetParentIdx() == pi) {
        return;
    }

    //要打乱link
    c->BreakLink();

    if (first_child_ == kInvalidIdx) {
        first_child_ = ci;
        c->SetParentIdx(pi);
    } else {
        uint16_t prev = kInvalidIdx;
        for (uint16_t next = first_child_; next != kInvalidIdx; ) {
            prev = next;
            next = transform_table_->GetComp(next)->GetNxtSiblingIdx();
        }

        transform_table_->GetComp(prev)->SetNxtSiblingIdx(ci);
        c->SetPreSiblingIdx(prev);
        c->SetParentIdx(pi);
    }
}

//断开链接,所有他指向的,和指向他的
//uint16_t parent_;
//uint16_t pre_sibling_;
//uint16_t nxt_sibling_;
void Transform::BreakLink()
{
    auto self_idx = transform_table_->GetCompIdx(entity_);
    if (parent_ != kInvalidIdx) {
        auto parent_comp = transform_table_->GetComp(parent_);
        if (parent_comp->GetFirstChildIdx() == self_idx) {
            parent_comp->SetFirstChildIdx(nxt_sibling_);
        }
    }

    if (pre_sibling_ != kInvalidIdx) {
        auto pre_sibling_comp = transform_table_->GetComp(pre_sibling_);
        pre_sibling_comp->SetNxtSiblingIdx(nxt_sibling_);
    }

    if (nxt_sibling_ != kInvalidIdx) {
        auto nxt_sibling_comp = transform_table_->GetComp(nxt_sibling_);
        nxt_sibling_comp->SetPreSiblingIdx(pre_sibling_);
    }
}

void Transform::ResetIdx()
{
    SetParentIdx(kInvalidIdx);
    SetPreSiblingIdx(kInvalidIdx);
    SetNxtSiblingIdx(kInvalidIdx);
    SetFirstChildIdx(kInvalidIdx);
}

void Transform::Relink(uint16_t old_idx, uint16_t new_idx)
{
    //uint16_t parent_;
    //uint16_t pre_sibling_;
    //uint16_t nxt_sibling_;
    //first_child_
    if (parent_ != kInvalidIdx) {
        auto parent_comp = transform_table_->GetComp(parent_);
        if (parent_comp->GetFirstChildIdx() == old_idx) {
            parent_comp->SetFirstChildIdx(new_idx);
        }
    }

    if (pre_sibling_ != kInvalidIdx) {
        auto pre_sibling_comp = transform_table_->GetComp(pre_sibling_);
        pre_sibling_comp->SetNxtSiblingIdx(new_idx);
    }

    if (nxt_sibling_ != kInvalidIdx) {
        auto nxt_sibling_comp = transform_table_->GetComp(nxt_sibling_);
        nxt_sibling_comp->SetPreSiblingIdx(new_idx);
    }

    if (first_child_ != kInvalidIdx) {
        auto first_child_comp = transform_table_->GetComp(first_child_);
        first_child_comp->SetParentIdx(new_idx);
    }
}

//void Transform::RemoveChild(Transform *c)
//{
//    auto pi = transform_table_->GetCompIdx(entity_);
//    auto ci = transform_table_->GetCompIdx(c->GetEntity());
//
//    if (c->GetParentIdx() != pi) {
//        return;
//    }
//    Info("nxt {}", c->GetNxtSiblingIdx());
//    if (first_child_ == ci) {
//        first_child_ = c->GetNxtSiblingIdx();
//    } else {
//        transform_table_->GetComp(c->GetPreSiblingIdx())\
//            ->SetNxtSiblingIdx(c->GetNxtSiblingIdx());
//    }
//
//    Info("nxt {}", c->GetNxtSiblingIdx());
//    auto nxt = c->GetNxtSiblingIdx();
//    if (nxt != kInvalidIdx) {
//        transform_table_->GetComp(nxt)\
//            ->SetPreSiblingIdx(c->GetPreSiblingIdx());
//    }
//    c->SetParentIdx(kInvalidIdx);
//    c->SetPreSiblingIdx(kInvalidIdx);
//    c->SetNxtSiblingIdx(kInvalidIdx);
//}

Transform * Transform::FirstChild()
{
    if (first_child_ != kInvalidIdx) {
        return transform_table_->GetComp(first_child_);
    }
    return nullptr;
}

Transform * Transform::Parent()
{
    if (parent_ != kInvalidIdx) {
        return transform_table_->GetComp(parent_);
    }
    return nullptr;
}

std::tuple<Transform *, Transform *>  Transform::Sibling()
{
    Transform *prev = nullptr;
    Transform *next = nullptr;
    if (pre_sibling_ != kInvalidIdx) {
        prev = transform_table_->GetComp(pre_sibling_);
    }
    if (nxt_sibling_ != kInvalidIdx) {
        next = transform_table_->GetComp(nxt_sibling_);
    }
    return std::make_tuple(prev, next);
}

void Transform::Reset()
{
    entity_ = Ghost;
    world_ = {math::Vec2(0,0), 0.0f, math::Vec2(0,0)};
    local_ = {math::Vec2(0,0), 0.0f, math::Vec2(0,0)};
    parent_  = kInvalidIdx;
    first_child_ = kInvalidIdx;
    pre_sibling_ = kInvalidIdx;
    nxt_sibling_ = kInvalidIdx;
    transform_table_ = nullptr;
}

void Transform::SetTransformTable(TransformTable *t)
{
    transform_table_ = t;
}

void Transform::CollectSubCompIdx(std::vector<int>& idx_list)
{
    // all child
    auto child = first_child_;
    while (child != kInvalidIdx) {
        idx_list.push_back(child);
        auto node = transform_table_->GetComp(child);
        node->CollectSubCompIdx(idx_list);
        child = node->GetNxtSiblingIdx();
    }
}


void Transform::dump()
{
    Info("entity_idx:{} - this_idx:{}- parent:{}-first_child:{}-pre_slib:{}--nxt:{}", entity_.Index(), transform_table_->GetCompIdx(entity_), parent_, first_child_, pre_sibling_, nxt_sibling_);
}
}

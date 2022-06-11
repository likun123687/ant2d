#include <gfx/Transform/transform.h>
#include <gfx/Transform/transform_table.h>

namespace ant2d {

Transform::Transform():transform_table_(nullptr)
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
    if (parent_ == 0) {
        SetPosition(nullptr, position);
    } else {
        SetPosition(&(transform_table_->GetComp(parent_)->GetWorld()), position);
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
    while (child != 0) {
        auto node = transform_table_->GetComp(child);
        node->SetPosition(&world_, node->GetLocal().position);
        child = node->GetNxtSiblingIdx();
    }
}

void Transform::SetScale(math::Vec2 scale)
{
    local_.scale = scale;
    if (parent_ == 0) {
        SetScale(nullptr, scale);
    } else {
        //SetScale(&transform_table.comps[parent_].world, scale);
        SetScale(&(transform_table_->GetComp(parent_)->GetWorld()), scale);
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
    while (child != 0) {
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
    if (parent_ == 0) {
        SetRotation(nullptr, rotation);
    } else {
        //SetRotation(&transform_table_.comps[parent_].world, rotation);
        SetRotation(&(transform_table_->GetComp(parent_)->GetWorld()), rotation);
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
    while (child != 0) {
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

    if (first_child_ == 0) {
        first_child_ = ci;
        c->SetParentIdx(pi);
    } else {
        uint16_t prev = 0;
        for (uint16_t next = first_child_; next != 0; 
            next = transform_table_->GetComp(next)->GetNxtSiblingIdx()) {
            prev = next;
        }

        transform_table_->GetComp(prev)->SetNxtSiblingIdx(ci);
        c->SetPreSiblingIdx(prev);
        c->SetParentIdx(pi);
    }
}

void Transform::RemoveChild(Transform *c)
{
    auto pi = transform_table_->GetCompIdx(entity_);
    auto ci = transform_table_->GetCompIdx(c->GetEntity());

    if (c->GetParentIdx() != pi) {
        return;
    }

    if (first_child_ == ci) {
        first_child_ = nxt_sibling_;
    } else {
        transform_table_->GetComp(c->GetPreSiblingIdx())\
            ->SetNxtSiblingIdx(c->GetNxtSiblingIdx());
    }

    auto nxt = c->GetNxtSiblingIdx();
    if (nxt != 0) {
        transform_table_->GetComp(nxt)\
            ->SetPreSiblingIdx(c->GetPreSiblingIdx());
    }
    c->SetParentIdx(0);
    c->SetPreSiblingIdx(0);
    c->SetNxtSiblingIdx(0);
}

Transform * Transform::FirstChild()
{
    if (first_child_ != 0) {
        return transform_table_->GetComp(first_child_);
    }
    return nullptr;
}

Transform * Transform::Parent()
{
    if (parent_ != 0) {
        return transform_table_->GetComp(parent_);
    }
    return nullptr;
}

std::tuple<Transform *, Transform *>  Transform::Sibling()
{
    Transform *prev = nullptr;
    Transform *next = nullptr;
    if (pre_sibling_) {
        prev = transform_table_->GetComp(pre_sibling_);
    }
    if (nxt_sibling_) {
        next = transform_table_->GetComp(nxt_sibling_);
    }
    return std::make_tuple(prev, next);
}

void Transform::Reset()
{
    if (parent_) {
        //transform_table_.comps[p].RemoveChild(this);
        transform_table_->GetComp(parent_)->RemoveChild(this);
    }
    entity_ = Ghost;
    world_ = {math::Vec2(0,0), 0.0f, math::Vec2(0,0)};
    local_ = {math::Vec2(0,0), 0.0f, math::Vec2(0,0)};
    parent_  = 0;
    first_child_ = 0;
    pre_sibling_ = 0;
    nxt_sibling_ = 0;
    transform_table_ = nullptr;
}

}

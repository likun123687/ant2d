#pragma once
#include <gfx/i_comp.h>
#include <math/vector.h>

namespace ant2d {
class TransformTable;

struct SRT
{
    math::Vec2 scale;
    float rotation;
    math::Vec2 position;
};

class Transform : public IComp
{
private:
    SRT world_;
    SRT local_;

    uint16_t parent_;
    uint16_t first_child_;
    uint16_t pre_sibling_;
    uint16_t nxt_sibling_;
    TransformTable *transform_table_;
public:
    Transform();
    uint16_t GetParentIdx();
    void SetParentIdx(uint16_t idx);

    uint16_t GetFirstChildIdx();
    void SetFirstChildIdx(uint16_t idx);

    uint16_t GetPreSiblingIdx();
    void SetPreSiblingIdx(uint16_t idx);

    uint16_t GetNxtSiblingIdx();
    void SetNxtSiblingIdx(uint16_t idx);

    math::Vec2 GetPosition();
    math::Vec2 GetScale();
    float GetRotation();
    SRT GetLocal();
    SRT GetWorld();
    void SetLocal(SRT local);
    void SetWorld(SRT world);
    void SetPosition(math::Vec2 position);
    void MoveBy(float dx, float dy);
    void SetPosition(const SRT *parent, math::Vec2 local);
    void SetScale(math::Vec2 scale);
    void SetScale(const SRT *parent, math::Vec2 scale);
    void ScaleBy(float dx, float dy);
    void SetRotation(float rotation);
    void SetRotation(const SRT *parent, float rotation);
    void RotateBy(float d);
    void LinkChildren(std::vector<Transform *> child_list);
    void LinkChild(Transform *c);
    //void RemoveChild(Transform *c);
    //void MoveChild(Transform *c, Transform *new_parent);
    Transform *FirstChild();
    Transform *Parent();
    std::tuple<Transform *, Transform *> Sibling();
    void Reset();
    void SetTransformTable(TransformTable *t);
    void dump();
    void CollectSubCompIdx(std::vector<int>& idx_list);
    void BreakLink();
    void ResetIdx();
    void Relink(uint16_t old_idx, uint16_t new_idx);
};
} //namespace ant2d

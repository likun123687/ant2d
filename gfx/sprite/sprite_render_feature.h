#pragma once
#include <gfx/batch_render.h>
#include <gfx/sprite/sprite.h>
#include <gfx/sprite/sprite_table.h>
#include <gfx/transform/transform.h>

namespace ant2d {
class SpriteBatchObject : public IBatchObject {
private:
    SpriteComp* sprite_comp_;
    Transform* transform_;

public:
    void Fill(std::vector<PosTexColorVertex>& buf);
    int Size();
    SpriteComp* GetSpriteComp()
    {
        return sprite_comp_;
    }
    void SetSpriteComp(SpriteComp* s)
    {
        sprite_comp_ = s;
    }
    Transform* GetTransform()
    {
        return transform_;
    }
    void SetTransform(Transform* t)
    {
        transform_ = t;
    }
};

class SpriteRenderFeature : public IRenderFeature {
private:
    int id_;
    BatchRender* batch_render_;
    SpriteTable* sprite_table_;
    TransformTable* transform_table_;

public:
    void SetRender(BatchRender* render);
    void SetTable(SpriteTable* sprite_table, TransformTable* transform_table);
    void Register(RenderSystem* rs);
    void Extract(View* v);
    void Draw(const std::vector<SortObject>& nodes);
};
}

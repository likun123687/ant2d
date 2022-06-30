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
    void Fill(std::vector<PosTexColorVertex>& buf, uint32_t vertex_pos);
    int Size();
    SpriteComp* GetSpriteComp();
    void SetSpriteComp(SpriteComp* s);
    Transform* GetTransform();
    void SetTransform(Transform* t);
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
    void Flush();
};
}

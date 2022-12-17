#pragma once
#include <gfx/batch_render.h>
#include <gfx/text/text_table.h>

namespace ant2d {

class TextBatchObject : public IBatchObject {
private:
    TextComp* text_comp_;
    Transform* transform_;

public:
    void Fill(std::vector<PosTexColorVertex>& buf, uint32_t vertex_pos);

    int Size();

    TextComp* GetTextComp();

    void SetTextComp(TextComp* s);

    Transform* GetTransform();

    void SetTransform(Transform* t);
};

class TextRenderFeature : public IRenderFeature {
private:
    int id_;
    BatchRender* batch_render_;
    TextTable* text_table_;
    TransformTable* transform_table_;

public:
    void SetRender(BatchRender* render);

    void SetTable(TextTable* text_table, TransformTable* transform_table);

    void Register(RenderSystem* rs);

    void Extract(View* v);

    void Draw(const std::vector<SortObject>& nodes);

    void Flush();
};
} // namespace ant2d

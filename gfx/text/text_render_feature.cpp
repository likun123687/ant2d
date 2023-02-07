#include <gfx/text/text_render_feature.h>
#include <gfx/render_system.h>
#include <gfx/transform/transform_table.h>

namespace ant2d {

void TextBatchObject::Fill(std::vector<PosTexColorVertex>& buf, uint32_t vertex_pos)
{
    auto srt = transform_->GetWorld();
    auto position = srt.position;

    auto [g_x, g_y] = text_comp_->Gravity();
    auto [width, height] = text_comp_->Size();

    auto ox = width * g_x;
    auto oy = height * g_y;
    // Transform matrix
    auto m = math::Mat3 {};
    m.Initialize(position[0], position[1], srt.rotation, srt.scale[0], srt.scale[1], ox, oy, 0, 0);

    auto& vertex = text_comp_->GetVertex();
    color::Byte4 color = text_comp_->Color();
    auto buf_begin = &buf[vertex_pos];

    for (int i = 0; i < vertex.size(); i++) {
        auto vi = i * 4;

        // auto v = &buf[vi + vertex_pos + 0];
        auto v = &buf_begin[vi + 0];
        std::tie(v->x, v->y) = m.Transform(vertex[i].x_offset, vertex[i].y_offset);
        v->u = vertex[i].region.x1;
        v->v = vertex[i].region.y2;

        v = &buf_begin[vi + 1];
        std::tie(v->x, v->y) = m.Transform(vertex[i].x_offset + vertex[i].w, vertex[i].y_offset);
        v->u = vertex[i].region.x2;
        v->v = vertex[i].region.y2;

        v = &buf_begin[vi + 2];
        std::tie(v->x, v->y) = m.Transform(vertex[i].x_offset + vertex[i].w, vertex[i].y_offset + vertex[i].h);
        v->u = vertex[i].region.x2;
        v->v = vertex[i].region.y1;

        v = &buf_begin[vi + 3];
        std::tie(v->x, v->y) = m.Transform(vertex[i].x_offset, vertex[i].y_offset + vertex[i].h);
        v->u = vertex[i].region.x1;
        v->v = vertex[i].region.y1;

        for (int j = 0; j < 4; j++) {
            buf_begin[vi + j].rgba = color;
        }
    }
}

int TextBatchObject::Size()
{
    return 4 * text_comp_->GetVertex().size();
}

TextComp* TextBatchObject::GetTextComp()
{
    return text_comp_;
}

void TextBatchObject::SetTextComp(TextComp* s)
{
    text_comp_ = s;
}

Transform* TextBatchObject::GetTransform()
{
    return transform_;
}

void TextBatchObject::SetTransform(Transform* t)
{
    transform_ = t;
}

void TextRenderFeature::SetRender(BatchRender* render)
{
    batch_render_ = render;
}

void TextRenderFeature::SetTable(TextTable* text_table, TransformTable* transform_table)
{
    text_table_ = text_table;
    transform_table_ = transform_table;
}

void TextRenderFeature::Register(RenderSystem* rs)
{
    Info("get table {}", 11);
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kBatchShader) {
            batch_render_ = reinterpret_cast<BatchRender*>(r.get());
        }
    }

    for (auto& t : (*rs->GetTableList())) {
        Info("get table {}", int(t->GetTableType()));
        if (t->GetTableType() == TableType::kText) {
            text_table_ = reinterpret_cast<TextTable*>(t.get());
        } else if (t->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(t.get());
        }
    }

    id_ = rs->Accept(this);
}

void TextRenderFeature::Extract(View* v)
{
    auto camera = v->camera;
    auto fi = uint32_t(id_) << 16;
    for (int i = 0; i < text_table_->GetSize(); i++) {
        auto text = text_table_->GetComp(i);
        auto transform = transform_table_->GetComp(text->GetEntity());
        auto [width, height] = text->Size();
        auto [g_x, g_y] = text->Gravity();
        if (text->Visible() && camera->InView(transform, math::Vec2 { width, height }, math::Vec2 { g_x, g_y })) {
            auto z_order = text->GetZOrder();
            auto batch_id = text->GetBatchId();
            auto sid = PackSortId(z_order.GetValue(), batch_id.GetValue());
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void TextRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    uint32_t sort_id = 0xFFFFFFFF;
    bool begin = false;

    // batch draw!
    auto text_batch_object = TextBatchObject {};
    for (auto& b : nodes) {
        auto ii = b.value & 0xFFFF; // 在text_table索引
        auto sid = b.sort_id & 0xFFFF; // batch_id也就是texid
        if (sort_id != sid) {
            if (begin) {
                batch_render_->End();
            }

            sort_id = sid;
            begin = true;
            uint16_t texture_id = 0;
            std::tie(texture_id, std::ignore) = text_table_->GetComp(ii)->Font()->Tex2D();
            uint16_t depth = 0; // 就是z_order
            std::tie(depth, std::ignore) = UnpackSortId(b.sort_id);
            batch_render_->Begin(texture_id, depth);
        }

        text_batch_object.SetTextComp(text_table_->GetComp(ii));
        auto entity = text_batch_object.GetTextComp()->GetEntity();
        text_batch_object.SetTransform(transform_table_->GetComp(entity));
        batch_render_->Draw(&text_batch_object);
    }

    if (begin) {
        batch_render_->End();
    }

    batch_render_->Flush();
}

void TextRenderFeature::Flush()
{
}
} // namespace ant2d

#include <gfx/render_system.h>
#include <gfx/sprite/sprite_render_feature.h>
#include <gfx/transform/transform_table.h>
#include <math/matrix.h>
#include <utils/debug.h>

namespace ant2d {
void SpriteBatchObject::Fill(std::vector<PosTexColorVertex>& buf, uint32_t vertex_pos)
{
    auto srt = transform_->GetWorld();
    auto position = srt.position;
    auto width = sprite_comp_->GetWidth();
    auto height = sprite_comp_->GetHeight();
    auto rg = sprite_comp_->GetTextureRegion();

    // Texture
    if (rg.rotated) {
        if (sprite_comp_->GetFlipX() == 1) {
            std::swap(rg.y1, rg.y2);
        }
        if (sprite_comp_->GetFlipY() == 1) {
            std::swap(rg.x1, rg.x2);
        }
        buf[vertex_pos + 1].u = rg.x1;
        buf[vertex_pos + 1].v = rg.y2;
        buf[vertex_pos + 2].u = rg.x2;
        buf[vertex_pos + 2].v = rg.y2;
        buf[vertex_pos + 3].u = rg.x2;
        buf[vertex_pos + 3].v = rg.y1;
        buf[vertex_pos + 0].u = rg.x1;
        buf[vertex_pos + 0].v = rg.y1;
    } else {
        if (sprite_comp_->GetFlipY() == 1) {
            std::swap(rg.y1, rg.y2);
        }
        if (sprite_comp_->GetFlipX() == 1) {
            std::swap(rg.x1, rg.x2);
        }
        buf[vertex_pos + 0].u = rg.x1;
        buf[vertex_pos + 0].v = rg.y2;
        buf[vertex_pos + 1].u = rg.x2;
        buf[vertex_pos + 1].v = rg.y2;
        buf[vertex_pos + 2].u = rg.x2;
        buf[vertex_pos + 2].v = rg.y1;
        buf[vertex_pos + 3].u = rg.x1;
        buf[vertex_pos + 3].v = rg.y1;
    }

    // Color
    // auto color = sprite_comp_->GetColor();
    // Info("sprite color {:#x}", color);
    auto rgba_color = sprite_comp_->GetColor();
    for (int i = 0; i < 4; i++) {
        buf[vertex_pos + i].rgba = color::Float4 { rgba_color };
    }

    // Center of model
    auto [g_x, g_y] = sprite_comp_->GetGravity();
    auto ox = width * g_x;
    auto oy = height * g_y;

    // Transform matrix
    auto m = math::Mat3 {};
    m.Initialize(position[0], position[1], srt.rotation, srt.scale[0], srt.scale[1], ox, oy, 0, 0);

    // Let's go!
    std::tie(buf[vertex_pos + 0].x, buf[vertex_pos + 0].y) = m.Transform(0, 0);
    std::tie(buf[vertex_pos + 1].x, buf[vertex_pos + 1].y) = m.Transform(width, 0);
    std::tie(buf[vertex_pos + 2].x, buf[vertex_pos + 2].y) = m.Transform(width, height);
    std::tie(buf[vertex_pos + 3].x, buf[vertex_pos + 3].y) = m.Transform(0, height);
}

int SpriteBatchObject::Size()
{
    return 4;
}

SpriteComp* SpriteBatchObject::GetSpriteComp()
{
    return sprite_comp_;
}

void SpriteBatchObject::SetSpriteComp(SpriteComp* s)
{
    sprite_comp_ = s;
}

Transform* SpriteBatchObject::GetTransform()
{
    return transform_;
}

void SpriteBatchObject::SetTransform(Transform* t)
{
    transform_ = t;
}

void SpriteRenderFeature::SetRender(BatchRender* render)
{
    batch_render_ = render;
}

void SpriteRenderFeature::SetTable(SpriteTable* sprite_table, TransformTable* transform_table)
{
    sprite_table_ = sprite_table;
    transform_table_ = transform_table;
}

void SpriteRenderFeature::Register(RenderSystem* rs)
{
    for (auto& r : rs->GetRenderList()) {
        if (r->GetType() == ShaderType::kBatchShader) {
            batch_render_ = reinterpret_cast<BatchRender*>(r.get());
        }
    }

    for (auto& t : (*rs->GetTableList())) {
        if (t->GetTableType() == TableType::kSprite) {
            sprite_table_ = reinterpret_cast<SpriteTable*>(t.get());
        } else if (t->GetTableType() == TableType::kTransform) {
            transform_table_ = reinterpret_cast<TransformTable*>(t.get());
        }
    }

    id_ = rs->Accept(this);
}

void SpriteRenderFeature::Extract(View* v)
{
    Info("extract sprite table size {}", sprite_table_->GetSize());
    auto camera = v->camera;
    auto fi = uint32_t(id_) << 16;
    for (int i = 0; i < sprite_table_->GetSize(); i++) {
        auto sprite = sprite_table_->GetComp(i);
        auto transform = transform_table_->GetComp(sprite->GetEntity());
        auto [gravity_width, gravity_height] = sprite->GetGravity();

        math::Vec2 sz = math::Vec2(sprite->GetWidth(), sprite->GetHeight());
        math::Vec2 g = math::Vec2(gravity_width, gravity_height);

        if (sprite->GetVisible() && camera->InView(transform, sz, g)) {
            auto z_order = sprite->GetZOrder();
            auto batch_id = sprite->GetBatchId(); // batch_id就是texid
            auto sid = PackSortId(z_order.GetValue(), batch_id.GetValue());
            auto value = fi + uint32_t(i);
            v->render_nodes.push_back(SortObject { sid, value });
        }
    }
}

void SpriteRenderFeature::Draw(const std::vector<SortObject>& nodes)
{
    uint32_t sort_id = 0xFFFFFFFF;
    bool begin = false;

    // batch draw!
    auto sprite_batch_object = SpriteBatchObject {};
    for (auto& b : nodes) {
        auto ii = b.value & 0xFFFF; // 在sprite_table索引
        auto sid = b.sort_id & 0xFFFF; // batch_id也就是texid
        if (sort_id != sid) {
            if (begin) {
                batch_render_->End();
            }

            sort_id = sid;
            begin = true;
            auto texture_id = sprite_table_->GetComp(ii)->GetTextureId();
            uint16_t depth = 0; // 就是z_order
            std::tie(depth, std::ignore) = UnpackSortId(b.sort_id);
            batch_render_->Begin(texture_id, depth);
        }

        sprite_batch_object.SetSpriteComp(sprite_table_->GetComp(ii));
        auto entity = sprite_batch_object.GetSpriteComp()->GetEntity();
        sprite_batch_object.SetTransform(transform_table_->GetComp(entity));
        batch_render_->Draw(&sprite_batch_object);
    }

    if (begin) {
        batch_render_->End();
    }

    batch_render_->Flush();
}

void SpriteRenderFeature::Flush()
{
}

}

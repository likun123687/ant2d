#include <gfx/render_system.h>
#include <gfx/sprite/sprite_render_feature.h>
#include <gfx/transform/transform_table.h>
#include <math/matrix.h>

namespace ant2d {
void SpriteBatchObject::Fill(std::vector<PosTexColorVertex>& buf)
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
        buf[1].u = rg.x1;
        buf[1].v = rg.y2;
        buf[2].u = rg.x2;
        buf[2].v = rg.y2;
        buf[3].u = rg.x2;
        buf[3].v = rg.y1;
        buf[0].u = rg.x1;
        buf[0].v = rg.y1;
    } else {
        if (sprite_comp_->GetFlipY() == 1) {
            std::swap(rg.y1, rg.y2);
        }
        if (sprite_comp_->GetFlipX() == 1) {
            std::swap(rg.x1, rg.x2);
        }
        buf[0].u = rg.x1;
        buf[0].v = rg.y2;
        buf[1].u = rg.x2;
        buf[1].v = rg.y2;
        buf[2].u = rg.x2;
        buf[2].v = rg.y1;
        buf[3].u = rg.x1;
        buf[3].v = rg.y1;
    }

    // Color
    auto color = sprite_comp_->GetColor();
    buf[0].rgba = color;
    buf[1].rgba = color;
    buf[2].rgba = color;
    buf[3].rgba = color;

    // Center of model
    auto [g_x, g_y] = sprite_comp_->GetGravity();
    auto ox = width * g_x;
    auto oy = height * g_y;

    // Transform matrix
    auto m = math::Mat3 {};
    m.Initialize(position[0], position[1], srt.rotation, srt.scale[0], srt.scale[1], ox, oy, 0, 0);

    // Let's go!
    std::tie(buf[0].x, buf[0].y) = m.Transform(0, 0);
    std::tie(buf[1].x, buf[1].y) = m.Transform(width, 0);
    std::tie(buf[2].x, buf[2].y) = m.Transform(width, height);
    std::tie(buf[3].x, buf[3].y) = m.Transform(0, height);
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
            auto batch_id = sprite->GetBatchId();
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
        auto ii = b.value & 0xFFFF;
        auto sid = b.sort_id & 0xFFFF;
        if (sort_id != sid) {
            if (begin) {
                batch_render_->End();
            }

            sort_id = sid;
            begin = true;
            auto texture_id = sprite_table_->GetComp(ii)->GetTextureId();
            uint16_t depth = 0;
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

void Flush()
{
}

}

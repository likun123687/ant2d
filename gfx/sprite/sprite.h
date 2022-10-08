#pragma once
#include <gfx/i_comp.h>
#include <gfx/bk_texture.h>
#include <gfx/render_data.h>
#include <utils/debug.h>

namespace ant2d {
class SpriteComp : public IComp {
public:
    SpriteComp();
    void SetSprite(ITexture2D* spt);
    void SetSize(float w, float h);
    std::tuple<float, float> GetSize();
    void SetGravity(float x, float y);
    std::tuple<float, float> GetGravity();
    void SetVisible(bool v);
    bool GetVisible();
    std::tuple<float, float, float, float> GetRgbaColor();
    // void SetColor(sg_color color);
    uint32_t GetColor();
    void SetColor(uint32_t color);
    void Flip(bool flip_x, bool flip_y);
    void Reset();
    float GetWidth();
    void SetWidth(float width);
    float GetHeight();
    void SetHeight(float height);
    Region GetTextureRegion();
    uint16_t GetFlipX();
    void SetFlipX(uint16_t flip_x);
    uint16_t GetFlipY();
    void SetFlipY(uint16_t flip_y);
    ZOrder GetZOrder()
    {
        return z_order_;
    }

    BatchId GetBatchId()
    {
        return batch_id_;
    }
    uint16_t GetTextureId()
    {
        Info("go inside get");
        Info("inside get texture id {}", static_cast<void*>(sprite_));
        return sprite_->GetTextureId();
    }

private:
    ITexture2D* sprite_;
    ZOrder z_order_;
    BatchId batch_id_;

    uint32_t color_;
    uint16_t flip_x_;
    uint16_t flip_y_;

    float width_;
    float height_;
    struct {
        float x;
        float y;
    } gravity_; // 重心
    bool visible_;
};
}

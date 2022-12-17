#include <gfx/sprite/sprite.h>
namespace ant2d {

SpriteComp::SpriteComp()
    : IComp()
    , sprite_(nullptr)
    , z_order_()
    , batch_id_()
    , color_(0)
    , flip_x_(0)
    , flip_y_(0)
    , width_(0)
    , height_(0)
    , gravity_ { 0.5, 0.5 }
    , visible_(false)
{
}

void SpriteComp::SetSprite(ITexture2D* spt)
{
    sprite_ = spt;
    // batch_id_.value = spt->GetTex();
    batch_id_.SetValue(sprite_->GetTextureId());
    if (width_ == 0 || height_ == 0) {
        auto size = sprite_->GetSize();
        width_ = size.width;
        height_ = size.height;
    }
}

void SpriteComp::SetSize(float w, float h)
{
    width_ = w;
    height_ = h;
}

std::tuple<float, float> SpriteComp::GetSize()
{
    return std::make_tuple(width_, height_);
}

void SpriteComp::SetGravity(float x, float y)
{
    gravity_.x = x;
    gravity_.y = y;
}

std::tuple<float, float> SpriteComp::GetGravity()
{
    return std::make_tuple(gravity_.x, gravity_.y);
}

void SpriteComp::SetVisible(bool v)
{
    visible_ = v;
}

bool SpriteComp::GetVisible()
{
    return visible_;
}

std::tuple<float, float, float, float> SpriteComp::GetRgbaColor()
{
    auto r = static_cast<uint8_t>(color_ >> 24) / 255.0f;
    auto g = static_cast<uint8_t>(color_ >> 16) / 255.0f;
    auto b = static_cast<uint8_t>(color_ >> 8) / 255.0f;
    auto a = static_cast<uint8_t>(color_ >> 0) / 255.0f;
    return { r, g, b, a };
}
// sg_color SpriteComp::GetColor()
//{
//     sg_color result;
//     result.r = static_cast<uint8_t>(color_>>24) / 255.0f;
//     result.g = static_cast<uint8_t>(color_>>16) / 255.0f;
//     result.b = static_cast<uint8_t>(color_>>8) / 255.0f;
//     result.a = static_cast<uint8_t>(color_>>0) / 255.0f;
//     return result;
// }

uint32_t SpriteComp::GetColor()
{
    return color_;
}

void SpriteComp::SetColor(uint32_t color)
{
    color_ = color;
}

// void SpriteComp::SetColor(sg_color color)
//{
//     uint8_t r = color.r * 255.0f;
//     uint8_t g = color.g * 255.0f;
//     uint8_t b = color.b * 255.0f;
//     uint8_t a = color.a * 255.0f;
//     color_ = (static_cast<uint32_t>(r)<<24 |
//             static_cast<uint32_t>(g)<<16 |
//             static_cast<uint32_t>(b)<<8 |
//             static_cast<uint32_t>(a)<<0);
// }

void SpriteComp::Flip(bool flip_x, bool flip_y)
{
    if (flip_x) {
        flip_x_ = 1;
    } else {
        flip_x_ = 0;
    }
    if (flip_y) {
        flip_y_ = 1;
    } else {
        flip_y_ = 0;
    }
}

void SpriteComp::Reset()
{
}

float SpriteComp::GetWidth()
{
    return width_;
}

void SpriteComp::SetWidth(float width)
{
    width_ = width;
}

float SpriteComp::GetHeight()
{
    return height_;
}

void SpriteComp::SetHeight(float height)
{
    height_ = height;
}

Region SpriteComp::GetTextureRegion()
{
    return sprite_->GetRegion();
}

uint16_t SpriteComp::GetFlipX()
{
    return flip_x_;
}

void SpriteComp::SetFlipX(uint16_t flip_x)
{
    flip_x_ = flip_x;
}

uint16_t SpriteComp::GetFlipY()
{
    return flip_y_;
}

void SpriteComp::SetFlipY(uint16_t flip_y)
{
    flip_y_ = flip_y;
}

} // namespace ant2d

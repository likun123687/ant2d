#include <gfx/text/text_comp.h>

namespace ant2d {

color::Byte4 TextComp::Color()
{
    return color_;
}

void TextComp::SetColor(color::Byte4 color)
{
    color_ = color;
}

const std::string& TextComp::Text()
{
    return text_;
}

void TextComp::SetText(const std::string& text)
{
    text_ = text;
    auto is_valid = ::utf8::is_valid(text.begin(), text.end());
    if (!is_valid) {
        Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
    }

    rune_count_ = ::utf8::unchecked::distance(text.begin(), text.end());
    FillData();
}

std::tuple<float, float> TextComp::Gravity()
{
    return { gravity_.x, gravity_.y };
}

void TextComp::SetGravity(float x, float y)
{
    gravity_.x = x;
    gravity_.y = y;
}

bool TextComp::Visible()
{
    return visible_;
}

void TextComp::SetVisible(bool v)
{
    visible_ = v;
}

float TextComp::FontSize()
{
    return size_;
}

void TextComp::SetFontSize(float sz)
{
    size_ = sz;
}

std::tuple<float, float> TextComp::Size()
{
    return { width_, height_ };
}

// generate text-vertex with the string
//
//		+----------+
//		| . 	   |
//      |   .	   |
//		|     .    |
// 		|		.  |
// 		+----------+
// 1 * 1 quad for each char
void TextComp::FillData()
{
    vertex_.resize(rune_count_);
    float gh = .0f;
    std::tie(std::ignore, gh) = font_->Bounds();

    float scale = size_ / gh;
    float x_offset = 0.0f;
    float y_offset = 0.0f;

    if (size_ == 0) {
        scale = 1.0f;
    } else {
        gh = size_;
    }

    y_offset = gh;

    auto iter = text_.begin();
    int i = 0;
    while (iter != text_.end()) {
        uint32_t r = ::utf8::unchecked::next(iter);
        auto glyph = font_->GetGlyph(r);
        if (!glyph) {
            Error("get glyph error {}", r);
        }

        auto advance = float(glyph->advance) * scale;
        auto vw = glyph->width * scale;
        auto vh = glyph->height * scale;
        auto ox = glyph->xoffset * scale;
        auto oy = glyph->yoffset * scale;

        auto [u1, v1, u2, v2] = font_->Frame(r);

        auto& item = vertex_[i];
        item.x_offset = x_offset + ox;
        item.y_offset = y_offset - (oy + vh);
        item.w = vw;
        item.h = vh;
        item.region.x1 = u1;
        item.region.y1 = v1;
        item.region.x2 = u2;
        item.region.y2 = v2;

        // left to right shift only!
        x_offset += advance;
        y_offset += 0;

        i++;
    }

    width_ = x_offset + vertex_.back().w;
    height_ = gh;
}

font::FontAtlas* TextComp::Font()
{
    return font_;
}

// should have default font!!
void TextComp::SetFont(font::FontAtlas* font)
{
    font_ = font;
    // if (font_ != nullptr && batch_id_.GetValue() != 0) {
    if (font_ != nullptr) {
        uint16_t tex_id = 0;
        std::tie(tex_id, std::ignore) = font_->Tex2D();
        batch_id_.SetValue(tex_id);
    }
}

} // namespace ant2d

#pragma once
#include <gfx/render_data.h>
#include <gfx/bk_texture.h>
#include <gfx/i_comp.h>
#include <gfx/font/font.h>
#include <string>
#include <third_party/utfcpp/utf8/unchecked.h>

namespace ant2d {
struct TextQuad {
    float x_offset;
    float y_offset;
    float w;
    float h;
    Region region;
};

class TextComp : public IComp {
public:
    color::Byte4 Color();

    void SetColor(color::Byte4 color);

    const std::string& Text();

    void SetText(const std::string& text);

    std::tuple<float, float> Gravity();
    void SetGravity(float x, float y);
    bool Visible();
    void SetVisible(bool v);

    float FontSize();

    void SetFontSize(float sz);

    std::tuple<float, float> Size();

    // generate text-vertex with the string
    //
    //		+----------+
    //		| . 	   |
    //      |   .	   |
    //		|     .    |
    // 		|		.  |
    // 		+----------+
    // 1 * 1 quad for each char
    void FillData();

    font::FontAtlas* Font();

    // should have default font!!
    void SetFont(font::FontAtlas* font);
    auto& GetVertex()
    {
        return vertex_;
    }

    ZOrder GetZOrder()
    {
        return z_order_;
    }
    
    BatchId GetBatchId()
    {
        return batch_id_;
    }

    void Reset()
    {
    }

private:
    font::FontAtlas* font_;
    ZOrder z_order_;
    BatchId batch_id_;
    float size_;
    color::Byte4 color_;

    float width_;
    float height_;
    struct {
        float x;
        float y;
    } gravity_;
    bool visible_;

    // TextModel
    std::string text_;
    std::vector<TextQuad> vertex_;
    int rune_count_;
};

} // namespace ant2d

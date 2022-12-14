#pragma once
#include <gfx/font/font.h>
#include <third_party/utfcpp/utf8/unchecked.h>

namespace ant2d {
class DrawList;
// 工具结构，负责把字符串转化为顶点..
// 拥有所有需要的条件属性
class FontRender {
public:
    FontRender(DrawList* draw_list, float font_size, font::FontAtlas* font, uint32_t color);

    // 当前的实现中，不考虑裁切优化，全部绘制所有字符
    math::Vec2 RenderText(math::Vec2 pos, const std::string& text);
    math::Vec2 RenderWrapped(math::Vec2 pos, const std::string& text, float wrap_width);

private:
    DrawList* draw_list_;
    float font_size_;
    font::FontAtlas* font_;
    uint32_t color_;
    float line_space_;
};
} // namespace ant2d

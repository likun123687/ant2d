#pragma once
#include <gfx/font/font.h>
#include <gfx/color.h>

namespace ant2d {
class DrawList;
// 工具结构，负责把字符串转化为顶点..
// 拥有所有需要的条件属性
class FontRender {
public:
    FontRender(DrawList* draw_list, float font_size, font::FontAtlas* font, color::Byte4 color);

    // 当前的实现中，不考虑裁切优化，全部绘制所有字符
    math::Vec2 RenderText(math::Vec2 pos, const std::string& text);
    math::Vec2 RenderWrapped(math::Vec2 pos, const std::string& text, float wrap_width);

private:
    DrawList* draw_list_;
    float font_size_;
    font::FontAtlas* font_;
    color::Byte4 color_;
    float line_space_;
};
} // namespace ant2d

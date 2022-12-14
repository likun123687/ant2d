#include <gui/font.h>
#include <gui/drawing.h>

namespace ant2d {

FontRender::FontRender(DrawList* draw_list, float font_size, font::FontAtlas* font, uint32_t color)
    : draw_list_(draw_list)
    , font_(font)
    , font_size_(font_size)
    , color_(color)
{
}

// 当前的实现中，不考虑裁切优化，全部绘制所有字符
math::Vec2 FontRender::RenderText(math::Vec2 pos, const std::string& text)
{
    auto is_valid = ::utf8::is_valid(text.begin(), text.end());
    if (!is_valid) {
        Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
    }

    int length = ::utf8::unchecked::distance(text.begin(), text.end());

    auto dx = pos[0];
    auto dy = pos[1];
    float max_width = 0;

    int idx_count = length * 6;
    int vtx_count = length * 4;

    draw_list_->PrimReserve(idx_count, vtx_count);

    auto vtx_writer = draw_list_->vtx_writer_;
    auto idx_writer = draw_list_->idx_writer_;

    auto color = color_;
    float gh = 0;
    std::tie(std::ignore, gh) = font_->Bounds();
    auto scale = font_size_ / gh;
    auto line_height = font_size_;
    auto line_space = line_space_;
    if (line_space == 0) {
        line_space = 0.4 * line_height;
    }

    auto buffer_used = 0;
    auto iter = text.begin();
    while (iter != text.end()) {
        uint32_t r = ::utf8::unchecked::next(iter);
        if (r == '\n') {
            if (dx > max_width) {
                max_width = dx;
            }

            dy -= line_height + line_space;
            dx = pos[0];
            continue;
        }

        auto g = font_->GetGlyph(r);
        auto x1 = dx;
        auto y1 = dy - (g->height + g->yoffset) * scale;

        auto x2 = x1 + g->width * scale;
        auto y2 = dy - (g->yoffset) * scale;

        auto [u1, v1, u2, v2] = font_->Frame(r);

        auto vi = buffer_used * 4;
        vtx_writer[vi + 0] = DrawVert { math::Vec2 { x1, y1 }, math::Vec2 { u1, v2 }, color };
        vtx_writer[vi + 1] = DrawVert { math::Vec2 { x2, y1 }, math::Vec2 { u2, v2 }, color };
        vtx_writer[vi + 2] = DrawVert { math::Vec2 { x2, y2 }, math::Vec2 { u2, v1 }, color };
        vtx_writer[vi + 3] = DrawVert { math::Vec2 { x1, y2 }, math::Vec2 { u1, v1 }, color };

        auto ii = buffer_used * 6;
        auto offset = draw_list_->idx_index_;
        idx_writer[ii + 0] = DrawIdx(offset + 0);
        idx_writer[ii + 1] = DrawIdx(offset + 1);
        idx_writer[ii + 2] = DrawIdx(offset + 2);
        idx_writer[ii + 3] = DrawIdx(offset + 0);
        idx_writer[ii + 4] = DrawIdx(offset + 2);
        idx_writer[ii + 5] = DrawIdx(offset + 3);

        draw_list_->idx_index_ += 6;
        draw_list_->vtx_index_ += 4;

        dx += float(g->advance) * scale;
        buffer_used++;
    }

    if (dx > max_width) {
        max_width = dx;
    }
    math::Vec2 size;
    size[0] = max_width - pos[0];
    size[1] = pos[1] - dy + font_size_;

    draw_list_->AddCommand(buffer_used * 6);
    return size;
}

math::Vec2 FontRender::RenderWrapped(math::Vec2 pos, const std::string& text, float wrap_width)
{
    auto wrap = math::Max(wrap_width, 0);
    std::string lines;
    std::tie(std::ignore, lines) = font::Wrap(font_, text, wrap, font_size_);
    auto size = RenderText(pos, lines);
    return size;
}
} // namespace ant2d

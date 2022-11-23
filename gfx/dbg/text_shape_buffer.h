#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>
#include <gfx/dbg/font.h>

namespace ant2d {
class TextShapeBuffer {
    TextShapeBuffer(uint32_t max_vertex)
        : vertex_ {}
    {
        uint32_t ibo_size = static_cast<uint32_t>(max_vertex * 6 / 4);
        index_.resize(ibo_size);
        uint16_t i_format[6] = { 3, 0, 1, 3, 1, 2 };
        for (int i = 0; i < ibo_size; i += 6) {
            std::copy(i_format, i_format + 6, index_.begin() + i);
            i_format[0] += 4;
            i_format[1] += 4;
            i_format[2] += 4;
            i_format[3] += 4;
            i_format[4] += 4;
            i_format[5] += 4;
        }

        std::tie(index_id_, ib_) = SharedResManager.AllocIndexBuffer((uint8_t*)index_.data(), ibo_size * 2);
        vertex_.resize(max_vertex);
        auto vbo_size = max_vertex * sizeof(PosTexColorVertex);
        std::tie(vertex_id_, vb_) = SharedResManager.AllocVertexBuffer(nullptr, max_vertex * vbo_size, vbo_size);

        // texture
        auto image_data = FontSystem::LoadFontImage();

        std::tie(font_tex_id_, std::ignore) = SharedResManager.AllocTexture(image_data);
        color_ = 0xFF000000;
    }

    void String(float x, float y, const std::string& chars, float scale)
    {
        auto w = FontSystem::font_width * scale;
        auto h = FontSystem::font_height * scale;
        int chars_len = chars.size();
        for (int i = 0; i < chars_len; i++) {
            auto b = &vertex_[pos_];
            pos_ += 4;
            auto [left, right, bottom, top] = FontSystem::GlyphRegion(chars[i]);
            b[0].x = x;
            b[0].y = y;
            b[0].u = left;
            b[0].v = bottom;

            b[1].x = x + w;
            b[1].y = y;
            b[1].u = right;
            b[1].v = bottom;

            b[2].x = x + w;
            b[2].y = y + h;
            b[2].u = right;
            b[2].v = top;

            b[3].x = x;
            b[3].y = y + h;
            b[3].u = left;
            b[3].v = top;
            for (int j = 0; j < 4; j++) {
                b[j].r = static_cast<uint8_t>(color_ >> 24) / 255.0f;
                b[j].g = static_cast<uint8_t>(color_ >> 16) / 255.0f;
                b[j].b = static_cast<uint8_t>(color_ >> 8) / 255.0f;
                b[j].a = static_cast<uint8_t>(color_ >> 0) / 255.0f;
            }
            // advance x,y
            x += w;
        }
    }

    //
    //  3-------2
    //  |       |
    //  |       |
    //  0-------1
    void Rect(float x, float y, float w, float h)
    {
        auto b = &vertex_[pos_];
        pos_ += 4;

        b[0].x = x;
        b[0].y = y;
        b[0].u = 2;
        b[0].v = 0;

        b[1].x = x + w;
        b[1].y = y;
        b[1].u = 2;
        b[1].v = 0;

        b[2].x = x + w;
        b[2].y = y + h;
        b[2].u = 2;
        b[2].v = 0;

        b[3].x = x;
        b[3].y = y + h;
        b[3].u = 2;
        b[3].v = 0;

        for (int j = 0; j < 4; j++) {
            b[j].r = static_cast<uint8_t>(color_ >> 24) / 255.0f;
            b[j].g = static_cast<uint8_t>(color_ >> 16) / 255.0f;
            b[j].b = static_cast<uint8_t>(color_ >> 8) / 255.0f;
            b[j].a = static_cast<uint8_t>(color_ >> 0) / 255.0f;
        }
    }

    void Line(math::Vec2 from, math::Vec2 to)
    {
        auto b = &vertex_[pos_];
        pos_ += 4;

        auto diff = to.Sub(from);
        auto inv_length = math::InvLength(diff[0], diff[1], 1.0);
        diff = diff.Mul(inv_length);
        float thickness = 1.0;

        auto dx = diff[1] * (thickness * 0.5);
        auto dy = diff[0] * (thickness * 0.5);

        b[0].x = from[0] + dx;
        b[0].y = from[1] - dy;
        b[0].u = 2;
        b[0].v = 0;

        b[1].x = from[0] + dx;
        b[1].y = from[1] - dy;
        b[1].u = 2;
        b[1].v = 0;

        b[2].x = from[0] - dx;
        b[2].y = from[1] + dy;
        b[2].u = 2;
        b[2].v = 0;

        b[3].x = from[0] - dx;
        b[3].y = from[1] + dy;
        b[3].u = 2;
        b[3].v = 0;

        for (int j = 0; j < 4; j++) {
            b[j].r = static_cast<uint8_t>(color_ >> 24) / 255.0f;
            b[j].g = static_cast<uint8_t>(color_ >> 16) / 255.0f;
            b[j].b = static_cast<uint8_t>(color_ >> 8) / 255.0f;
            b[j].a = static_cast<uint8_t>(color_ >> 0) / 255.0f;
        }
    }

    void Border(float x, float y, float w, float h, float thick)
    {
        Rect(x, y, w, thick);
        Rect(x, y + h - thick, w, thick);
        Rect(x, y, thick, h);
        Rect(x + w - thick, y, thick, h);
    }

    void Circle(float x, float y, float radius)
    {
        uint16_t segments = 12;
        auto path = std::array<math::Vec2, 24> {};
        auto angle = float(3.14 * 2);

        if (radius < 4) {
            segments = 4;
        } else if (radius < 100) {
            segments = uint16_t(radius / 100 * 16) + 8;
        } else {
            segments = 24;
        }

        for (int i = 0; i < segments; i++) {
            auto a = float(i) / float(segments) * angle;
            auto x1 = x + math::Cos(a) * radius;
            auto y1 = y + math::Sin(a) * radius;
            path[i] = math::Vec2 { x1, y1 };
        }

        for (int i = 0; i < segments; i++) {
            auto j = i + 1;
            if (j == segments) {
                j = 0;
            }
            auto p1 = path[i];
            auto p2 = path[j];
            Line(p1, p2);
        }
    }

private:
    std::vector<PosTexColorVertex> vertex_;
    std::vector<uint16_t> index_;

    // gpu res
    uint16_t index_id_;
    uint16_t vertex_id_;
    IndexBuffer* ib_;
    VertexBuffer* vb_;
    uint16_t font_tex_id_;

    // current painter color
    uint32_t color_;

    // current buffer position
    uint32_t pos_;
};
} // namespace debug

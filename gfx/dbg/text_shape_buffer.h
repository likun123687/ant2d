#pragma once
#include <gfx/render_data.h>
#include <gfx/bk/res_manager.h>
#include <gfx/dbg/font.h>
#include <gfx/dbg/config.h>

namespace ant2d {
class TextShapeBuffer {
public:
    TextShapeBuffer(uint32_t max_vertex);
    void String(float x, float y, const std::string& chars, float scale);

    //
    //  3-------2
    //  |       |
    //  |       |
    //  0-------1
    void Rect(float x, float y, float w, float h);
    void Line(math::Vec2 from, math::Vec2 to);
    void Border(float x, float y, float w, float h, float thick);
    void Circle(float x, float y, float radius);
    void Update();
    void Reset();
    void Destroy();
    uint16_t GetVertexId();
    uint16_t GetIndexId();
    uint16_t GetPos();
    uint16_t GetFontTexId();
    void SetColor(uint32_t color);

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
} // namespace ant2d


#pragma once
#include <vector>
#include <string>

#include <math/vector.h>
#include <gfx/font/font.h>
#include <math/common_func.h>
#include <gfx/bk/res_manager.h>
#include <gfx/color.h>
#include <gfx/render_data.h>

namespace ant2d {
constexpr uint32_t FlagAntiAliasedLine = 0;
constexpr uint32_t FlagAntiAliasedFill = 1;

// Rounding corner:
// A: 0x0000 0001 top-left
// B: 0x0000 0002 top-right
// C: 0x0000 0004 down-right
// D: 0x0000 0008 down-left
using FlagCorner = uint32_t;
constexpr FlagCorner FlagCornerNone = 0x0000;
constexpr FlagCorner FlagCornerTopLeft = 0x0001;
constexpr FlagCorner FlagCornerTopRight = 0x0002;
constexpr FlagCorner FlagCornerBottomRight = 0x0004;
constexpr FlagCorner FlagCornerBottomLeft = 0x0008;
constexpr FlagCorner FlagCornerAll = 0x000F;

constexpr uint32_t AlignCenter = 0;
constexpr uint32_t AlignLeft = 1 << 1;
constexpr uint32_t AlignRight = 1 << 2;
constexpr uint32_t AlignTop = 1 << 3;
constexpr uint32_t AlignBottom = 1 << 4;

constexpr int16_t DefaultZOrder = int16_t((0xFFFF >> 1) - 100);

// DrawList provide method to write primitives to buffer
struct DrawCmd {
    uint16_t first_index;
    uint16_t elem_count;
    math::Vec4 clip_rect;
    uint16_t texture_id;
    uint16_t z_order;
};

struct DrawVert {
    math::Vec2 xy;
    math::Vec2 uv;
    color::Float4 color;
};

static_assert(std::is_pod<DrawVert>::value, "DrawVert must be pod");
static_assert(sizeof(DrawVert) == sizeof(PosTexColorVertex), "drawvert and PosTexColorVertex must be same size");

using DrawIdx = uint16_t;

class DrawList {
    // clang-format off
    static constexpr uint16_t NinePatchIndex[54] = {
     	0, 1, 5, 0, 5, 4, 1, 2, 6, 1, 6, 5, 2, 3, 7, 2, 7, 6,
        4, 5, 9, 4, 9, 8, 5, 6, 10, 5, 10, 9, 6, 7, 11, 6, 11, 10,
        8, 9, 13, 8, 13, 12, 9, 10, 14, 9, 14, 13, 10, 11, 15, 10, 15, 14,
    };
    // clang-format on

public:
    std::vector<DrawCmd> cmd_buffer_;
    std::vector<DrawIdx> idx_buffer_;
    std::vector<DrawVert> vtx_buffer_;

    int cmd_index_;
    int idx_index_;
    int vtx_index_;
    int cmd_cap_;
    int idx_cap_;
    int vtx_cap_;

    // Data *DrawListSharedData
    std::string owner_name_; // 窗口名
    int vtx_current_idx_; // VtxBuffer.Size

    // 指向当前正在使用的 cmdbuffer 的位置
    DrawVert* vtx_writer_;
    int reserve_vtx_count_;
    DrawIdx* idx_writer_;
    int reserve_idx_count_;

    std::vector<math::Vec4> clip_rect_stack_;
    std::vector<uint16_t> texture_id_stack_;

    // path_
    std::array<math::Vec2, 64> path_;
    int path_used_;

    math::Vec4 full_screen_;
    math::Vec2 tex_uv_white_pixel_;
    std::array<math::Vec2, 12> circle_vtx12_;
    font::FontAtlas* font_;
    float font_size_;

    uint32_t flags_;
    int16_t z_order_;

public:
    DrawList();

    bool Empty();

    std::tuple<int, int> Size();

    // TODO
    void Clear();

    void PathClear();

    void PathLineTo(math::Vec2 pos);

    void PathLineToMergeDuplicate(math::Vec2 pos);

    // primitive operation, auto scale by 1024
    void PrimReserve(int idx_count, int vtx_count);

    void PrimRect(math::Vec2 min, math::Vec2 max, uint32_t color);

    void PrimRectUV(math::Vec2 a, math::Vec2 c, math::Vec2 uva, math::Vec2 uvc, uint32_t color);

    void PrimQuadUV(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d,
        math::Vec2 uva, math::Vec2 uvb, math::Vec2 uvc, math::Vec2 uvd, uint32_t color);

    // 此处生成最终的顶点数据和索引数据
    // 当前并不支持抗锯齿！！简单的用顶点生成线段
    void AddPolyLine(math::Vec2* points, int point_count, uint32_t color, float thickness, bool closed);

    // Non Anti-aliased Fill
    void AddConvexPolyFilled(math::Vec2* points, int point_count, uint32_t color);

    // 此处圆角的算法：
    // 使用一个12边形近似圆形，采用中心放射算法，计算出
    // 各个角度的sin/cos, 然后通过公式，得到圆圆形顶点
    // f(x) = centre.x + cos()*radius
    // f(y) = centre.y + sin()*radius
    // 以上, 可以提前算好 sin/cos 加速整个过程
    void PathArcToFast(math::Vec2 centre, float radius, int min12, int max12);

    void PathArcTo(math::Vec2 centre, float radius, float min, float max, int segments);

    void PathBezierCurveTo(math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, int segments);

    void PathRect(math::Vec2 a, math::Vec2 b, float rounding, FlagCorner corners);

    void AddLine(math::Vec2 a, math::Vec2 b, uint32_t color, float thickness);

    // 所有非填充图形看来都是使用路径实现的
    void AddRect(math::Vec2 a, math::Vec2 b, uint32_t color, float rounding, FlagCorner roundFlags, float thickness);

    void AddRectFilled(math::Vec2 min, math::Vec2 max, uint32_t color, float rounding, FlagCorner corner);

    void AddRectFilledMultiColor();

    void AddQuad(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d, uint32_t color, float thickness);

    void AddQuadFilled(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d, uint32_t color);

    void AddTriangle(math::Vec2 a, math::Vec2 b, math::Vec2 c, uint32_t color, float thickness);

    void AddTriangleFilled(math::Vec2 a, math::Vec2 b, math::Vec2 c, uint32_t color);

    void AddCircle(math::Vec2 centre, float radius, uint32_t color, int segments, float thickness);

    void AddCircleFilled(math::Vec2 centre, float radius, uint32_t color, int segments);

    void AddBezierCurve(math::Vec2 pos0, math::Vec2 cp0, math::Vec2 cp1, math::Vec2 pos1,
        uint32_t color, float thickness, int segments);

    void AddImage(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 uva, math::Vec2 uvb, uint32_t color);

    void AddImageQuad(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d,
        math::Vec2 uva, math::Vec2 uvb, math::Vec2 uvc, math::Vec2 uvd, uint32_t color);

    void AddImageRound(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 uva, math::Vec2 uvb,
        uint32_t color, float rounding, FlagCorner corners);

    // NinePatch Algorithm
    //  12   13   14   15
    //       x1   x2     max
    //  +----+----+----+
    //  |    |    |    |
    //  |    |    |p1  |
    //  +----+----+----+ y2
    //  |    |    |    |
    //  |    |p0  |    |
    //  +----+----+----+ y1
    //  |    |    |    |
    //  |    |    |    |
    //  +----+----+----+
    // min
    //  0    1    2    3
    // patch = {x1, x2, y1, y2} % TextureSize
    void AddImageNinePatch(uint16_t tex_id, math::Vec2 min, math::Vec2 max, math::Vec2 uva, math::Vec2 uvb,
        math::Vec4 patch, uint32_t color);

    void PathFillConvex(uint32_t col);

    void PathStroke(uint32_t color, float thickness, bool closed);

    math::Vec4 CurrentClipRect();

    uint16_t CurrentTextureId();

    // will result in new draw-call
    void UpdateClipRect();

    void UpdateTextureId();

    // Clip 相关的操作
    void PushClipRect(math::Vec2 min, math::Vec2 max, bool intersect_current_clip);

    void PushClipRectFullScreen();
    void PopClipRect();

    math::Vec2 GetClipRectMin();

    math::Vec2 GetClipRectMax();

    void PushTextureId(uint16_t tex_id);

    void PopTextureId();

    math::Vec2 AddText(math::Vec2 pos, const std::string& text, font::FontAtlas* font, float font_size, uint32_t color, float wrap_width);

    // 每次绘制都会产生一个 Command （可能会造成内存浪费! 1k cmd = 1000 * 6 * 4 = 24k）
    // 为了减少内存可以一边添加一边尝试向前合并
    void AddCommand(int elem_count);

    DrawCmd* GetCommands();
};

} // namespace ant2d


#include <gui/drawing.h>
#include <gui/font.h>

namespace ant2d {
DrawList::DrawList()
{
    cmd_buffer_.resize(1024);
    idx_buffer_.resize(2024);
    vtx_buffer_.resize(2024);

    tex_uv_white_pixel_ = math::Vec2 { 0, 0 };

    for (int i = 0; i < 12; i++) {
        auto sin = math::Sin((6.28 / 12) * float(i));
        auto cos = math::Cos((6.28 / 12) * float(i));
        circle_vtx12_[i] = math::Vec2 { cos, sin };
    }

    z_order_ = DefaultZOrder;
    cmd_index_ = 1; // skip first one
}

bool DrawList::Empty()
{
    return vtx_index_ == 0 || idx_index_ == 0;
}

std::tuple<int, int> DrawList::Size()
{
    return { idx_index_, vtx_index_ };
}

// TODO
void DrawList::Clear()
{
    cmd_index_ = 1;
    idx_index_ = 0;
    vtx_index_ = 0;
}

void DrawList::PathClear()
{
    path_used_ = 0;
}

void DrawList::PathLineTo(math::Vec2 pos)
{
    auto n = path_.size();
    if (path_used_ < n - 1) {
        path_[path_used_] = pos;
        path_used_ += 1;
    }
}

void DrawList::PathLineToMergeDuplicate(math::Vec2 pos)
{
}

// primitive operation, auto scale by 1024
void DrawList::PrimReserve(int idx_count, int vtx_count)
{
    int sz = 0;
    int require = 0;

    sz = vtx_buffer_.size();
    require = vtx_index_ + vtx_count;
    if (require >= sz) {
        vtx_buffer_.resize(sz + 1024);
    }

    sz = idx_buffer_.size();
    require = idx_index_ + idx_count;
    if (require >= sz) {
        idx_buffer_.resize(sz + 1024);
    }

    vtx_writer_ = &vtx_buffer_[vtx_index_];
    idx_writer_ = &idx_buffer_[idx_index_];
    reserve_idx_count_ = idx_count;
    reserve_vtx_count_ = vtx_count;
}

void DrawList::PrimRect(math::Vec2 min, math::Vec2 max, uint32_t color)
{
    auto uv = tex_uv_white_pixel_;
    auto a = min;
    auto b = math::Vec2 { max[0], min[1] };
    auto c = max;
    auto d = math::Vec2 { min[0], max[1] };

    vtx_writer_[0] = DrawVert { a, uv, color };
    vtx_writer_[1] = DrawVert { b, uv, color };
    vtx_writer_[2] = DrawVert { c, uv, color };
    vtx_writer_[3] = DrawVert { d, uv, color };

    idx_writer_[0] = DrawIdx(vtx_index_ + 0);
    idx_writer_[1] = DrawIdx(vtx_index_ + 1);
    idx_writer_[2] = DrawIdx(vtx_index_ + 2);

    idx_writer_[3] = DrawIdx(vtx_index_ + 0);
    idx_writer_[4] = DrawIdx(vtx_index_ + 2);
    idx_writer_[5] = DrawIdx(vtx_index_ + 3);

    vtx_index_ += 4;
    idx_index_ += 6;
}

void DrawList::PrimRectUV(math::Vec2 a, math::Vec2 c, math::Vec2 uva, math::Vec2 uvc, uint32_t color)
{
    auto b = math::Vec2 { c[0], a[1] };
    auto d = math::Vec2 { a[0], c[1] };
    auto uvb = math::Vec2 { uvc[0], uva[1] };
    auto uvd = math::Vec2 { uva[0], uvc[1] };

    vtx_writer_[0] = DrawVert { a, uva, color };
    vtx_writer_[1] = DrawVert { b, uvb, color };
    vtx_writer_[2] = DrawVert { c, uvc, color };
    vtx_writer_[3] = DrawVert { d, uvd, color };

    auto ii = vtx_index_;
    idx_writer_[0] = DrawIdx(ii + 0);
    idx_writer_[1] = DrawIdx(ii + 1);
    idx_writer_[2] = DrawIdx(ii + 2);
    idx_writer_[3] = DrawIdx(ii + 0);
    idx_writer_[4] = DrawIdx(ii + 2);
    idx_writer_[5] = DrawIdx(ii + 3);

    vtx_index_ += 4;
    idx_index_ += 6;
}

void DrawList::PrimQuadUV(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d,
    math::Vec2 uva, math::Vec2 uvb, math::Vec2 uvc, math::Vec2 uvd, uint32_t color)
{
    // vertex
    vtx_writer_[0] = DrawVert { a, uva, color };
    vtx_writer_[1] = DrawVert { b, uvb, color };
    vtx_writer_[2] = DrawVert { c, uvc, color };
    vtx_writer_[3] = DrawVert { d, uvd, color };

    auto ii = vtx_index_;
    idx_writer_[0] = DrawIdx(ii + 0);
    idx_writer_[1] = DrawIdx(ii + 1);
    idx_writer_[2] = DrawIdx(ii + 2);
    idx_writer_[3] = DrawIdx(ii + 0);
    idx_writer_[4] = DrawIdx(ii + 2);
    idx_writer_[5] = DrawIdx(ii + 3);

    vtx_index_ += 4;
    idx_index_ += 6;
}

// 此处生成最终的顶点数据和索引数据
// 当前并不支持抗锯齿！！简单的用顶点生成线段
void DrawList::AddPolyLine(math::Vec2* points, int point_count, uint32_t color, float thickness, bool closed)
{
    if (point_count < 2) {
        return;
    }
    auto uv = tex_uv_white_pixel_;
    auto count = point_count;
    if (!closed) {
        count = point_count - 1;
    }
    // Non Anti-aliased Stroke
    auto idx_count = count * 6;
    auto vtx_count = count * 4;
    PrimReserve(idx_count, vtx_count);

    for (int i1 = 0; i1 < count; i1++) {
        auto i2 = i1 + 1;
        if (i2 == point_count) {
            i2 = 0;
        }

        auto p1 = points[i1];
        auto p2 = points[i2];

        auto diff = p2.Sub(p1);

        float inv_length = math::InvLength(diff[0], diff[1], 1.0);
        diff = diff.Mul(inv_length);
        float dx = diff[0] * (thickness * 0.5);
        float dy = diff[1] * (thickness * 0.5);

        auto vi = i1 * 4;
        vtx_writer_[vi + 0] = DrawVert { math::Vec2 { p1[0] + dy, p1[1] - dx }, uv, color };
        vtx_writer_[vi + 1] = DrawVert { math::Vec2 { p2[0] + dy, p2[1] - dx }, uv, color };
        vtx_writer_[vi + 2] = DrawVert { math::Vec2 { p2[0] - dy, p2[1] + dx }, uv, color };
        vtx_writer_[vi + 3] = DrawVert { math::Vec2 { p1[0] - dy, p1[1] + dx }, uv, color };

        auto ii = i1 * 6;
        idx_writer_[ii + 0] = DrawIdx(vtx_index_ + 0);
        idx_writer_[ii + 1] = DrawIdx(vtx_index_ + 1);
        idx_writer_[ii + 2] = DrawIdx(vtx_index_ + 2);
        idx_writer_[ii + 3] = DrawIdx(vtx_index_ + 0);
        idx_writer_[ii + 4] = DrawIdx(vtx_index_ + 2);
        idx_writer_[ii + 5] = DrawIdx(vtx_index_ + 3);

        vtx_index_ += 4;
        idx_index_ += 6;
    }
    AddCommand(idx_count);
}

// Non Anti-aliased Fill
void DrawList::AddConvexPolyFilled(math::Vec2* points, int point_count, uint32_t color)
{
    auto& uv = tex_uv_white_pixel_;
    auto idx_count = (point_count - 2) * 3;
    auto vtx_count = point_count;
    PrimReserve(idx_count, vtx_count);

    for (int i = 0; i < vtx_count; i++) {
        vtx_writer_[i] = DrawVert { points[i], uv, color };
    }

    for (int i = 2, ii = 0; i < point_count; i += 1, ii += 3) {
        idx_writer_[ii + 0] = DrawIdx(vtx_index_ + 0);
        idx_writer_[ii + 1] = DrawIdx(vtx_index_ + i - 1);
        idx_writer_[ii + 2] = DrawIdx(vtx_index_ + i);
    }

    vtx_index_ += vtx_count;
    idx_index_ += idx_count;
    AddCommand(idx_count);
}

// 此处圆角的算法：
// 使用一个12边形近似圆形，采用中心放射算法，计算出
// 各个角度的sin/cos, 然后通过公式，得到圆圆形顶点
// f(x) = centre.x + cos()*radius
// f(y) = centre.y + sin()*radius
// 以上, 可以提前算好 sin/cos 加速整个过程
void DrawList::PathArcToFast(math::Vec2 centre, float radius, int min12, int max12)
{
    if (radius == 0 || min12 > max12) {
        path_[path_used_] = centre;
        path_used_++;
        return;
    }
    for (auto a = min12; a <= max12; a++) {
        auto x = centre[0] + circle_vtx12_[a % 12][0] * radius;
        auto y = centre[1] + circle_vtx12_[a % 12][1] * radius;
        path_[path_used_] = math::Vec2 { x, y };
        path_used_++;
    }
}

void DrawList::PathArcTo(math::Vec2 centre, float radius, float min, float max, int segments)
{
    if (radius == 0) {
        path_[path_used_] = centre;
        path_used_++;
        return;
    }

    for (auto i = 0; i <= segments; i++) {
        auto a = min + (float(i) / float(segments)) * (max - min);
        auto x = centre[0] + math::Cos(a) * radius;
        auto y = centre[1] + math::Sin(a) * radius;
        path_[path_used_] = math::Vec2 { x, y };
        path_used_++;
    }
}

void DrawList::PathBezierCurveTo(math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, int segments)
{
}

void DrawList::PathRect(math::Vec2 a, math::Vec2 b, float rounding, FlagCorner corners)
{
    if (rounding <= 0 || corners == FlagCornerNone) {
        PathLineTo(a);
        PathLineTo(math::Vec2 { b[0], a[1] });
        PathLineTo(b);
        PathLineTo(math::Vec2 { a[0], b[1] });
    } else {
        float bl, br, tr, tl;
        if ((corners & FlagCornerBottomLeft) != 0) {
            bl = rounding;
        }
        if ((corners & FlagCornerBottomRight) != 0) {
            br = rounding;
        }
        if ((corners & FlagCornerTopRight) != 0) {
            tr = rounding;
        }
        if ((corners & FlagCornerTopLeft) != 0) {
            tl = rounding;
        }
        PathArcToFast(math::Vec2 { a[0] + bl, a[1] + bl }, bl, 6, 9); // bottom-left
        PathArcToFast(math::Vec2 { b[0] - br, a[1] + br }, br, 9, 12); // bottom-right
        PathArcToFast(math::Vec2 { b[0] - tr, b[1] - tr }, tr, 0, 3); // top-right
        PathArcToFast(math::Vec2 { a[0] + tl, b[1] - tl }, tl, 3, 6); // top-left
    }
}

void DrawList::AddLine(math::Vec2 a, math::Vec2 b, uint32_t color, float thickness)
{
    PathLineTo(a.Add(math::Vec2 { .5, .5 }));
    PathLineTo(b.Add(math::Vec2 { .5, .5 }));
    PathStroke(color, thickness, false);
}

// 所有非填充图形看来都是使用路径实现的
void DrawList::AddRect(math::Vec2 a, math::Vec2 b, uint32_t color, float rounding, FlagCorner roundFlags, float thickness)
{
    // dl.PathRect(a.Add(mgl32.Vec2{5, .5}), b.Sub(mgl32.Vec2{.5, .5}), rounding, roundFlags)
    //  TODO
    PathRect(a, b, rounding, roundFlags);
    PathStroke(color, thickness, true);
}

void DrawList::AddRectFilled(math::Vec2 min, math::Vec2 max, uint32_t color, float rounding, FlagCorner corner)
{
    if (rounding > 0 && corner != FlagCornerNone) {
        PathRect(min, max, rounding, corner);
        PathFillConvex(color);
    } else {
        PrimReserve(6, 4);
        PrimRect(min, max, color);
        AddCommand(6);
    }
}

void DrawList::AddRectFilledMultiColor()
{
}

void DrawList::AddQuad(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d, uint32_t color, float thickness)
{
    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathStroke(color, thickness, true);
}

void DrawList::AddQuadFilled(math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d, uint32_t color)
{
    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathFillConvex(color);
}

void DrawList::AddTriangle(math::Vec2 a, math::Vec2 b, math::Vec2 c, uint32_t color, float thickness)
{
    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathStroke(color, thickness, true);
}

void DrawList::AddTriangleFilled(math::Vec2 a, math::Vec2 b, math::Vec2 c, uint32_t color)
{
    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathFillConvex(color);
}

void DrawList::AddCircle(math::Vec2 centre, float radius, uint32_t color, int segments, float thickness)
{
    auto max = math::PI * 2 * float(segments - 1) / float(segments);
    PathArcTo(centre, radius, 0.0, max, segments);
    PathStroke(color, thickness, true);
}

void DrawList::AddCircleFilled(math::Vec2 centre, float radius, uint32_t color, int segments)
{
    auto max = math::PI * 2 * float(segments - 1) / float(segments);
    PathArcTo(centre, radius, 0.0, max, segments);
    PathFillConvex(color);
}

void DrawList::AddBezierCurve(math::Vec2 pos0, math::Vec2 cp0, math::Vec2 cp1, math::Vec2 pos1,
    uint32_t color, float thickness, int segments)
{
    PathLineTo(pos0);
    PathBezierCurveTo(cp0, cp1, pos1, segments);
    PathStroke(color, thickness, false);
}

void DrawList::AddImage(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 uva, math::Vec2 uvb, uint32_t color)
{
    auto n = texture_id_stack_.size();
    bool stack_changed = false;
    if (n == 0 || tex_id != texture_id_stack_[n - 1]) {
        stack_changed = true;
        PushTextureId(tex_id);
    }

    PrimReserve(6, 4);
    PrimRectUV(a, b, uva, uvb, color);
    AddCommand(6);
    if (stack_changed) {
        PopTextureId();
    }
}

void DrawList::AddImageQuad(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 c, math::Vec2 d,
    math::Vec2 uva, math::Vec2 uvb, math::Vec2 uvc, math::Vec2 uvd, uint32_t color)
{
    auto n = texture_id_stack_.size();
    bool stack_changed = false;
    if (n == 0 || tex_id != texture_id_stack_[n - 1]) {
        stack_changed = true;
        PushTextureId(tex_id);
    }

    PrimReserve(6, 4);
    PrimQuadUV(a, b, c, d, uva, uvb, uvc, uvd, color);
    AddCommand(6);
    if (stack_changed) {
        PopTextureId();
    }
}

void DrawList::AddImageRound(uint16_t tex_id, math::Vec2 a, math::Vec2 b, math::Vec2 uva, math::Vec2 uvb,
    uint32_t color, float rounding, FlagCorner corners)
{
    if (rounding <= 0 || (corners & FlagCornerAll) == 0) {
        AddImage(tex_id, a, b, uva, uvb, color);
        return;
    }

    auto n = texture_id_stack_.size();
    bool stack_changed = false;
    if (n == 0 || tex_id != texture_id_stack_[n - 1]) {
        stack_changed = true;
        PushTextureId(tex_id);
    }

    PathRect(a, b, rounding, corners);
    PathFillConvex(color);

    // map uv to vertex - linear scale
    auto xy_size = b.Sub(a);
    auto uv_size = uvb.Sub(uva);
    math::Vec2 scale { 0, 0 };

    if (xy_size[0] != 0) {
        scale[0] = uv_size[0] / xy_size[0];
    }
    if (xy_size[1] != 0) {
        scale[1] = uv_size[1] / xy_size[1];
    }

    // clamp??
    for (int i = 0; i < reserve_vtx_count_; i++) {
        auto vertex = vtx_writer_ + i;
        auto dx = (vertex->xy[0] - a[0]) * scale[0];
        auto dy = (vertex->xy[1] - a[1]) * scale[1];
        vertex->uv = math::Vec2 { uva[0] + dx, uva[1] + dy };
    }
    if (stack_changed) {
        PopTextureId();
    }
}

void DrawList::AddImageNinePatch(uint16_t tex_id, math::Vec2 min, math::Vec2 max, math::Vec2 uva, math::Vec2 uvb,
    math::Vec4 patch, uint32_t color)
{
    auto n = texture_id_stack_.size();
    bool stack_changed = false;
    if (n == 0 || tex_id != texture_id_stack_[n - 1]) {
        stack_changed = true;
        PushTextureId(tex_id);
    }

    Texture2D* tex = SharedResManager.GetTexture(tex_id);
    auto tex_size = math::Vec2 { tex->GetWidth(), tex->GetHeight() };

    int idx_count = 9 * 6;
    int vtx_count = 16;
    PrimReserve(idx_count, vtx_count);

    auto x1 = min[0] + patch[0] * tex_size[0];
    auto x2 = max[0] - patch[1] * tex_size[0];
    auto y1 = min[1] + patch[2] * tex_size[1];
    auto y2 = max[1] - patch[3] * tex_size[1];

    auto uvw = uvb[0] - uva[0];
    auto uvh = uvb[1] - uva[1];
    auto u1 = uva[0] + patch[0] * uvw;
    auto u2 = uvb[0] - patch[1] * uvw;
    auto v1 = uva[1] + patch[2] * uvh;
    auto v2 = uvb[1] - patch[3] * uvh;

    if (x2 < x1) {
        x1 = (min[0] + max[0]) / 2;
        x2 = x1;
    }

    if (y2 < y1) {
        y1 = (min[1] + max[1]) / 2;
        y2 = y1;
    }

    // fill vertex
    vtx_writer_[0] = DrawVert { min, uva, color };
    vtx_writer_[1] = DrawVert { math::Vec2 { x1, min[1] }, math::Vec2 { u1, uva[1] }, color };
    vtx_writer_[2] = DrawVert { math::Vec2 { x2, min[1] }, math::Vec2 { u2, uva[1] }, color };
    vtx_writer_[3] = DrawVert { math::Vec2 { max[0], min[1] }, math::Vec2 { uvb[0], uva[1] }, color };

    vtx_writer_[4] = DrawVert { math::Vec2 { min[0], y1 }, math::Vec2 { uva[0], v1 }, color };
    vtx_writer_[5] = DrawVert { math::Vec2 { x1, y1 }, math::Vec2 { u1, v1 }, color };
    vtx_writer_[6] = DrawVert { math::Vec2 { x2, y1 }, math::Vec2 { u2, v1 }, color };
    vtx_writer_[7] = DrawVert { math::Vec2 { max[0], y1 }, math::Vec2 { uvb[0], v1 }, color };

    vtx_writer_[8] = DrawVert { math::Vec2 { min[0], y2 }, math::Vec2 { uva[0], v2 }, color };
    vtx_writer_[9] = DrawVert { math::Vec2 { x1, y2 }, math::Vec2 { u1, v2 }, color };
    vtx_writer_[10] = DrawVert { math::Vec2 { x2, y2 }, math::Vec2 { u2, v2 }, color };
    vtx_writer_[11] = DrawVert { math::Vec2 { max[0], y2 }, math::Vec2 { uvb[0], v2 }, color };

    vtx_writer_[12] = DrawVert { math::Vec2 { min[0], max[1] }, math::Vec2 { uva[0], uvb[1] }, color };
    vtx_writer_[13] = DrawVert { math::Vec2 { x1, max[1] }, math::Vec2 { u1, uvb[1] }, color };
    vtx_writer_[14] = DrawVert { math::Vec2 { x2, max[1] }, math::Vec2 { u2, uvb[1] }, color };
    vtx_writer_[15] = DrawVert { max, uvb, color };

    // fill index
    auto ii = uint16_t(vtx_index_);
    for (int i = 0; i < 54; i++) {
        idx_writer_[i] = DrawIdx(ii + NinePatchIndex[i]);
    }

    idx_index_ += idx_count;
    vtx_index_ += vtx_count;

    AddCommand(idx_count);
}

void DrawList::PathFillConvex(uint32_t col)
{
    AddConvexPolyFilled(&path_[0], path_used_, col);
    path_used_ = 0;
}

void DrawList::PathStroke(uint32_t color, float thickness, bool closed)
{
    AddPolyLine(&path_[0], path_used_, color, thickness, closed);
    PathClear();
}

math::Vec4 DrawList::CurrentClipRect()
{
    math::Vec4 clip;
    auto size = clip_rect_stack_.size();
    if (size > 0) {
        clip = clip_rect_stack_[size - 1];
    } else {
        clip = full_screen_;
    }

    return clip;
}

uint16_t DrawList::CurrentTextureId()
{
    uint16_t id = 0;
    auto n = texture_id_stack_.size();
    if (n > 0) {
        id = texture_id_stack_[n - 1];
    }

    return id;
}

// will result in new draw-call
void DrawList::UpdateClipRect()
{
    // clip := dl.CurrentClipRect()
}

void DrawList::UpdateTextureId()
{
}

// Clip 相关的操作
void DrawList::PushClipRect(math::Vec2 min, math::Vec2 max, bool intersect_current_clip)
{
    auto cr = math::Vec4 { min[0], min[1], max[0], max[1] };
    if (intersect_current_clip && clip_rect_stack_.size() > 0) {
        auto& current = clip_rect_stack_.back();
        if (cr[0] < current[0]) {
            cr[0] = current[0];
        }
        if (cr[1] < current[1]) {
            cr[1] = current[1];
        }
        if (cr[2] > current[2]) {
            cr[2] = current[2];
        }
        if (cr[3] > current[3]) {
            cr[3] = current[3];
        }
        cr[2] = math::Max(cr[0], cr[2]);
        cr[3] = math::Max(cr[1], cr[3]);

        clip_rect_stack_.push_back(cr);
        UpdateClipRect();
    }
}

void DrawList::PushClipRectFullScreen()
{
    auto min = math::Vec2 { full_screen_[0], full_screen_[1] };
    auto max = math::Vec2 { full_screen_[2], full_screen_[3] };
    PushClipRect(min, max, false);
}

void DrawList::PopClipRect()
{
    clip_rect_stack_.pop_back();
}

math::Vec2 DrawList::GetClipRectMin()
{
    return math::Vec2 { 0, 0 };
}

math::Vec2 DrawList::GetClipRectMax()
{
    return math::Vec2 { 0, 0 };
}

void DrawList::PushTextureId(uint16_t tex_id)
{
    texture_id_stack_.push_back(tex_id);
}

void DrawList::PopTextureId()
{
    texture_id_stack_.pop_back();
}

math::Vec2 DrawList::AddText(math::Vec2 pos, const std::string& text, font::FontAtlas* font, float font_size, uint32_t color, float wrap_width)
{
    if (text.length() == 0) {
        return { 0, 0 };
    }

    if (font == nullptr) {
        font = font_;
    }

    if (font_size == 0) {
        font_size = font_size_;
    }

    FontRender fr(
        this,
        font_size,
        font,
        color);

    math::Vec2 size;
    if (wrap_width > 0) {
        size = fr.RenderWrapped(pos, text, wrap_width);
    } else {
        size = fr.RenderText(pos, text);
    }
    return size;
}

// 每次绘制都会产生一个 Command （可能会造成内存浪费! 1k cmd = 1000 * 6 * 4 = 24k）
// 为了减少内存可以一边添加一边尝试向前合并
void DrawList::AddCommand(int elem_count)
{
    math::Vec4 clip = CurrentClipRect();
    uint16_t tex = CurrentTextureId();
    uint16_t order = z_order_;
    int index = cmd_index_;

    DrawCmd& prev = cmd_buffer_[index - 1];
    if (prev.clip_rect == clip && prev.texture_id == tex && prev.z_order == order) {
        prev.elem_count += elem_count;
    } else {
        uint16_t fi = prev.first_index + prev.elem_count;
        cmd_buffer_[index] = DrawCmd { fi, uint16_t(elem_count), clip, tex, order };
        cmd_index_ += 1;
    }
}

DrawCmd* DrawList::GetCommands()
{
    return &cmd_buffer_[1];
}
} // namespace ant2d

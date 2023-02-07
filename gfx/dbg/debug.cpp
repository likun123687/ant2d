#include <gfx/dbg/debug.h>
#include <gfx/dbg/debug_render.h>
#include <utils/debug.h>

namespace ant2d {

void Debug::DrawFps(float x, float y)
{
    buffer_.SetColor({ 0x00, 0x00, 0x00, 0xFF });
    buffer_.Rect(x + 5, y + 5, 50, 6);

    // format: RGBA
    buffer_.SetColor({ 0x00, 0xFF, 0x00, 0xFF });

    auto w = float(fps_) / 60 * 50;
    buffer_.Rect(x + 5, y + 5, w, 5);

    buffer_.SetColor({ 0x00, 0x00, 0x00, 0xFF });
    buffer_.String(float(x + 5), float(y + 10), std::to_string(fps_) + "fps", .6f);
}

void Debug::DrawDrawCall(float x, float y)
{
    buffer_.String(x + 5, y + 25, std::string("DrawCall:") + std::to_string(draw_call_), .6f);
}

Debug::Debug()
    : debug_render_ { kDebugShader }
    , buffer_ { debug_render_.GetBuffer() }
{
}

void Debug::Draw()
{
    auto& view = debug_render_.GetView();
    auto x = view.x - view.w / 2;
    auto y = view.y - view.h / 2;

    // draw fps & draw call
    if ((dbg::DEBUG & dbg::FPS) != 0) {
        DrawFps(x, y);
        DrawDrawCall(x, y);
    }

    // draw string
    if ((dbg::DEBUG & dbg::Stats) != 0) {
        float d = 0.0f;
        x += 10;
        y += view.h - 20;

        for (auto& str : verbs_) {
            buffer_.String(x, y - d, str, .6f);
            d += 10;
        }
    }
}

void Debug::LogFPS(int fps, int draw_call)
{
    Info("debug fps {}=={}", fps, draw_call);
    fps_ = fps;
    draw_call_ = draw_call;
    if (dbg::DEBUG != dbg::None) {
        draw_call_ = draw_call - 1;
    }
}

void Debug::Reset()
{
    verbs_.resize(0);
}

void Debug::Hud(const std::string& str)
{
    verbs_.push_back(str);
}

void Debug::SetCamera(float x, float y, float w, float h)
{
    debug_render_.SetViewPort(x, y, w, h);
}

void Debug::SetColor(color::Byte4 rgba)
{
    buffer_.SetColor(rgba);
}

// draw a rect
void Debug::DrawRect(float x, float y, float w, float h)
{
    if (dbg::DEBUG & dbg::Draw) {
        buffer_.Rect(x, y, w, h);
    }
}

void Debug::DrawBorder(float x, float y, float w, float h, float thickness)
{
    if (dbg::DEBUG & dbg::Draw) {
        buffer_.Border(x, y, w, h, thickness);
    }
}

// draw a circle
void Debug::DrawCircle(float x, float y, float r)
{
    if (dbg::DEBUG & dbg::Draw) {
        buffer_.Circle(x, y, r);
    }
}

void Debug::DrawLine(math::Vec2 from, math::Vec2 to)
{
    if (dbg::DEBUG & dbg::Draw) {
        buffer_.Line(from, to);
    }
}

// draw string
void Debug::DrawStr(float x, float y, const std::string& str, float scale)
{
    if (dbg::DEBUG & dbg::Draw) {
        buffer_.String(x, y, str, scale);
    }
}

void Debug::AdvanceFrame()
{
    // draw hud
    Draw();
    Reset();

    // flush
    buffer_.Update();
    debug_render_.Draw();
    buffer_.Reset();
}
} // namespace ant2d

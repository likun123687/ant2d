#pragma once
#include <gfx/dbg/debug_render.h>

namespace ant2d {
class Debug {
private:
    DebugRender debug_render_;
    TextShapeBuffer& buffer_;
    std::vector<std::string> verbs_;

    int draw_call_;
    int fps_;

private:
    void DrawFps(float x, float y);
    void DrawDrawCall(float x, float y);

public:
    Debug();
    void Draw();

    void LogFPS(int fps, int draw_call);

    void Reset();
    void Hud(const std::string& str);

    void SetCamera(float x, float y, float w, float h);

    void SetColor(color::Byte4 rgba);

    // draw a rect
    void DrawRect(float x, float y, float w, float h);

    void DrawBorder(float x, float y, float w, float h, float thickness);
    // draw a circle
    void DrawCircle(float x, float y, float r);

    void DrawLine(math::Vec2 from, math::Vec2 to);
    // draw string
    void DrawStr(float x, float y, const std::string& str, float scale = 1.0);
    void AdvanceFrame();
    void Destroy() {}
};
} // namespace debug

#define SharedDebug \
    ::ant2d::Singleton<::ant2d::Debug>()
#pragma once
namespace ant2d {
class WindowCallback {
public:
    // 窗口创建
    virtual void OnCreate(float w, float h, float pixel_ratio) = 0;

    // Resize...
    virtual void OnResize(float w, float h) = 0;

    // 窗口循环
    virtual void OnLoop() = 0;

    // 窗口销毁
    virtual void OnDestroy() = 0;

    // 窗口切回？
    virtual void OnResume() = 0;

    // 窗口切入后台
    virtual void OnPause() = 0;

    // 窗口焦点变化
    virtual void OnFocusChanged(bool focused) = 0;

    virtual void OnKeyEvent(int key, bool pressed) = 0;
    virtual void OnPointEvent(int key, bool pressed, float x, float y) = 0;
    virtual ~WindowCallback() = default;
};
}

#pragma once
#include <hid/config.h>
#include <hid/life_cycle.h>
#include "sokol_app.h"

namespace ant2d {
class HidManager
{
public:
    WindowCallback *window_callback_;
    std::array<int, 1024> keys_;
    WindowOptions options_;
    void SetWindowCallback(WindowCallback *callback);
    void SetOptions(WindowOptions options);
    void InitCallback();
    void FrameCallback();
    void CleanupCallback();
    void EventCallback(const sapp_event* e);
};
}

#define SharedHidManager \
    ::ant2d::Singleton<::ant2d::HidManager>()

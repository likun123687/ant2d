#pragma once
#include "sokol_app.h"
#include <hid/config.h>
#include <hid/life_cycle.h>
#include <memory>

namespace ant2d {
class HidManager {
public:
    WindowCallback* window_callback_;
    std::array<int, 1024> keys_;
    std::unique_ptr<WindowOptions> options_;
    sg_pass_action pass_action_;
    void SetWindowCallback(WindowCallback* callback);
    void SetOptions(WindowOptions* options);
    void InitCallback();
    void FrameCallback();
    void CleanupCallback();
    void EventCallback(const sapp_event* e);
};
}

#define SharedHidManager \
    ::ant2d::Singleton<::ant2d::HidManager>()

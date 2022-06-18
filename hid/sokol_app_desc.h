#include <hid/hid_manager.h>

namespace ant2d {
namespace hid {
    static void InitCallback()
    {
        SharedHidManager.InitCallback();
    }

    static void FrameCallback()
    {
        SharedHidManager.FrameCallback();
    }

    static void CleanupCallback()
    {
        SharedHidManager.CleanupCallback();
    }
    static void EventCallback(const sapp_event* e)
    {
        SharedHidManager.EventCallback(e);
    }
} //namespace hid
} //namespace ant2d

sapp_desc sokol_main(int argc, char* argv[])
{
    auto options = ant2d_main(argc, argv);
    sapp_desc desc = {};
    desc.init_cb = ::ant2d::hid::InitCallback;
    desc.frame_cb = ::ant2d::hid::FrameCallback;
    desc.cleanup_cb = ::ant2d::hid::CleanupCallback;
    desc.event_cb = ::ant2d::hid::EventCallback;
    if (options.width > 0 && options.height > 0) {
        desc.width = options.width;
        desc.height = options.height;
    }
    desc.window_title = options.title;
    desc.win32_console_utf8 = true;
    desc.win32_console_attach = true;
    return desc;
}

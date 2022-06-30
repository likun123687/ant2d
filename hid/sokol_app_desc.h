#pragma once
#include <hid/hid_manager.h>


#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"
#include <utils/debug.h>

namespace ant2d {
namespace hid {
    static void InitCallback()
    {
        Info("InitCallback");
        SharedHidManager.InitCallback();
    }

    static void FrameCallback()
    {
        Info("FrameCallback");
        SharedHidManager.FrameCallback();
    }

    static void CleanupCallback()
    {
        Info("CleanupCallback");
        SharedHidManager.CleanupCallback();
    }
    static void EventCallback(const sapp_event* e)
    {
        Info("EventCallback");
        SharedHidManager.EventCallback(e);
    }
} //namespace hid
} //namespace ant2d

extern ant2d::WindowOptions ant2d_main(int argc, char* argv[]);

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
    desc.window_title = options.title.c_str();
    desc.win32_console_utf8 = true;
    desc.win32_console_attach = true;
    Info("sokol main called!!");
    return desc;
}

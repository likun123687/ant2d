#include <gfx/bk/sokol_gfx_bk.h>
#include <hid/hid_manager.h>
#include <utils/debug.h>
namespace ant2d {
void HidManager::SetWindowCallback(WindowCallback* callback)
{
    Info("set windows callback {}", static_cast<void*>(callback));
    window_callback_ = callback;
}

void HidManager::SetOptions(WindowOptions options)
{
    options_ = options;
}

void HidManager::InitCallback()
{
    float width = sapp_widthf();
    float height = sapp_heightf();
    window_callback_->OnCreate(width, height, sapp_dpi_scale());
    window_callback_->OnResume();
}

void HidManager::FrameCallback()
{
    window_callback_->OnLoop();
}

void HidManager::CleanupCallback(void)
{
    sg_shutdown();
}

void HidManager::EventCallback(const sapp_event* e)
{
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
    }
}
} // namespace ant2d

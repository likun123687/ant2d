#include <gfx/bk/sokol_gfx_bk.h>
#include <hid/hid_manager.h>
#include <utils/debug.h>
namespace ant2d {

void HidManager::SetWindowCallback(WindowCallback* callback)
{
    window_callback_ = callback;
}

void HidManager::SetOptions(WindowOptions* options)
{
    options_.reset(options);
}

void HidManager::InitCallback()
{
    /* a pass action to clear framebuffer */
    pass_action_.colors[0].action = SG_ACTION_CLEAR;
    pass_action_.colors[0].value = { 1.0f, 1.0f, 1.0f, 1.0f };
    float width = sapp_widthf();
    float height = sapp_heightf();
    window_callback_->OnCreate(width, height, sapp_dpi_scale());
    window_callback_->OnResume();
}

void HidManager::FrameCallback()
{
    sg_begin_default_pass(&pass_action_, sapp_width(), sapp_height());
    window_callback_->OnLoop();
    sg_end_pass();
    sg_commit();
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
        window_callback_->OnKeyEvent(e->key_code, true);
    } else if (e->type == SAPP_EVENTTYPE_KEY_UP) {
        window_callback_->OnKeyEvent(e->key_code, false);

    } else if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN
        || e->type == SAPP_EVENTTYPE_MOUSE_UP) {
        int pb = 0;
        if (e->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            pb = SAPP_MOUSEBUTTON_LEFT;
        } else if (e->mouse_button == SAPP_MOUSEBUTTON_MIDDLE) {
            pb = SAPP_MOUSEBUTTON_MIDDLE;
        } else if (e->mouse_button == SAPP_MOUSEBUTTON_RIGHT) {
            pb = SAPP_MOUSEBUTTON_RIGHT;
        }

        bool is_pressed = false;
        if (e->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
            is_pressed = true;
        }

        Info("mouse cb {}:{}:{}:{}", pb, is_pressed, e->mouse_x, e->mouse_y);
        window_callback_->OnPointEvent(pb, is_pressed, e->mouse_x, e->mouse_y);
    }
}
} // namespace ant2d

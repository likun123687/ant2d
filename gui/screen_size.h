#pragma once
#include <utils/singleton.h>
namespace ant2d {

class ScreenSize {
public:
    float rl_width_;
    float rl_height_;
    float vt_width_;
    float vt_height_;
    float hint_x_;
    float scale_x_;
    float hint_y_;
    float scale_y_;

    void SetRealSize(float w, float h)
    {
        rl_width_ = w;
        rl_height_ = h;
        UpdateHint();
    }

    void SetVirtualSize(float w, float h)
    {
        vt_width_ = w;
        vt_height_ = h;
        UpdateHint();
    }

    void UpdateHint()
    {
        if (rl_width_ == 0 || rl_height_ == 0) {
            return;
        }

        // update hint
        auto w = vt_width_;
        auto h = vt_height_;
        if (w == 0 && h == 0) {
            hint_x_ = rl_width_;
            hint_y_ = rl_height_;
            scale_x_ = 1;
            scale_y_ = 1;
        } else if (w == 0) {
            auto f = rl_height_ / h;
            scale_y_ = f;
            scale_x_ = f;
            hint_y_ = h;
            hint_x_ = rl_width_ / f;
        } else if (h == 0) {
            auto f = rl_width_ / w;
            scale_y_ = f;
            scale_x_ = f;
            hint_x_ = w;
            hint_y_ = rl_height_ / f;
        } else {
            scale_x_ = rl_width_ / w;
            scale_y_ = rl_height_ / h;
            hint_x_ = w;
            hint_y_ = h;
        }
    }
};
} // namespace ant2d

#define SharedScreen \
    ant2d::Singleton<ant2d::ScreenSize>()

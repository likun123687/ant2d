#include <effect/rate_controller.h>

namespace ant2d {
void RateController::Initialize(float du, float rate)
{
    duration_ = du;
    if (rate == 0) {
        thresh_time_ = 1.0f / 60;
    } else {
        thresh_time_ = 1.0f / rate;
    }
}

int RateController::Rate(float dt)
{
    int n = 0;
    life_time_ += dt;
    if (stop_ || life_time_ > duration_) {
        return n;
    }
    acc_time_ += dt;
    if (acc_time_ >= thresh_time_) {
        auto acc = acc_time_;
        auto d = thresh_time_;
        while (acc > d) {
            acc -= d;
            n++;
        }
        acc_time_ = acc;
    }
    return n;
}

void RateController::Stop()
{
    stop_ = true;
}

void RateController::Play()
{
    stop_ = false;
    life_time_ = 0;
}

void RateController::Prewarm(float t)
{
    warmup_time_ = t;
}

float RateController::WarmTime()
{
    return warmup_time_;
}

} // namespace ant2d

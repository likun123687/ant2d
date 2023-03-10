#include <game/fps.h>

namespace ant2d {
FPS::FPS()
    : pre_time_ {}
    , dt_ { 0 }
    , pause_ { false }
{
}

void FPS::Init()
{
    start_time_ = std::chrono::system_clock::now();
}

void FPS::SetScale(float factor)
{
    scale_ = factor;
}

float FPS::Step()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> dt = now - pre_time_;
    pre_time_ = now;
    dt_ = dt.count();
    fps_ = static_cast<int32_t>(1 / dt_);
    return dt_;
}

float FPS::Smooth()
{
    auto now = std::chrono::system_clock::now();
    auto du = now - pre_time_;
    pre_time_ = now;

    float dt = float(1.0 / 60);
    std::chrono::seconds cmp_s(3);
    if (du < cmp_s) {
        typedef std::chrono::duration<float> float_seconds;
        dt = std::chrono::duration_cast<float_seconds>(du).count();
    }

    auto predt = dt_;
    auto sdt = predt * 0.8 + static_cast<float>(dt * 0.2);

    dt_ = float(sdt);
    fps_ = static_cast<int32_t>(1 / dt);
    realdt_ = dt;
    return dt_;
}

void FPS::Pause()
{
    pause_ = true;
}

void FPS::Resume()
{
    pause_ = false;
}

float FPS::GetFPS()
{
    return fps_;
}

}

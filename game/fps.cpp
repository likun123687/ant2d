#include <fps.h>
using namespace game;

void FPS::initialize() {
    startTime_ = time.Now();
}

void FPS::SetScale(float factor) {
    scale_ = factor
}

float FPS::Step() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> dt = now - preTime_;
    preTime_ = now;
    dt_ = dt.count();
    fps_ = static_cast<int32_t>(1/dt);
}

float FPS::Smooth()
{
    auto now = std::chrono::system_clock::now();
    auto du = now - preTime_;
    preTime_ = now;

    float dt = 1.0/60;
    chrono::seconds cmp_s(3);
    if (du < cmp_s) {
        typedef std::chrono::duration<float> float_seconds;
        dt = std::chrono::duration_cast<float_seconds>(du).count();
    }

    auto predt = dt_;
    auto sdt = predt * 0.8 + static_cast<float>(dt * 0.2);

    dt_ = sdt;
    fps_ = static_cast<int32_t>(1/dt);
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


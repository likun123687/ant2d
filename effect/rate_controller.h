#pragma once
#include <effect/common.h>

namespace ant2d {
class RateController : public IController {

public:
    // Initialize init RateController with duration and emitter-rate.
    void Initialize(float du, float rate);

    int Rate(float dt);

    void Stop();

    void Play();

    void Prewarm(float t);

    float WarmTime();

private:
    float warmup_time_ = 0.0f;
    // control emitter-rate
    float acc_time_ = 0.0f;
    float thresh_time_ = 0.0f;

    // lifetime
    float life_time_ = 0.0f;
    float duration_ = 0.0f;
    bool stop_ = false;
};
} // namespace ant2d

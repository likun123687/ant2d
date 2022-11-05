#pragma once
namespace ant2d {
class RateController {

public:
    // Initialize init RateController with duration and emitter-rate.
    void Initialize(float du, float rate);

    int Rate(float dt);

    void Stop();

    void Play();

    void Prewarm(float t);

    float WarmTime();

private:
    float warmup_time_;
    // control emitter-rate
    float acc_time_;
    float thresh_time_;

    // lifetime
    float life_time_;
    float duration_;
    bool stop_;
};
} // namespace ant2d

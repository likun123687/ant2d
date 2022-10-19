#pragma once
#include <chrono>
#include <ratio>
#include <thread>

namespace ant2d {
class FPS {
public:
    FPS();
    void Init();
    void SetScale(float factor);
    float Step();
    float Smooth();
    void Pause();
    void Resume();

private:
    std::chrono::system_clock::time_point start_time_;
    std::chrono::system_clock::time_point pre_time_;
    float dt_;
    float scale_;
    float realdt_;
    int32_t fps_;
    bool pause_;
};

template <std::intmax_t FPS>
class FrameRater {
public:
    FrameRater()
        : // initialize the object keeping the pace
        time_between_frames_ { 1 }
        , // std::ratio<1, FPS> seconds
        tp_ { std::chrono::steady_clock::now() }
    {
    }

    void Sleep()
    {
        // add to time point
        tp_ += time_between_frames_;

        // and sleep until that time point
        std::this_thread::sleep_until(tp_);
    }

private:
    // a duration with a length of 1/FPS seconds
    std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames_;

    // the time point we'll add to in every loop
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames_)> tp_;
};
}

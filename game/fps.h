#pragma once
#include <chrono>

namespace ant2d {
    class FPS {
    public:
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
}


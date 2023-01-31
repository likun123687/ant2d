#pragma once
#include <cstdint>
#include <math/ease/elastic.h>

namespace ant2d {
namespace ween {
    struct TweenEngine;
    using LoopType = uint8_t;
    using UpdateCallback = std::function<void(bool, float)>;
    using EndCallback = std::function<void(bool)>;

    struct Animator {
        TweenEngine* en = nullptr;
        int index = 0;
        Animator() = default;
        Animator(TweenEngine* en_, int index_);
        Animator* SetDuration(float d_);
        Animator* SetRepeat(int count_, LoopType loop_);
        Animator* SetFunction(math::ease::Function function_);
        Animator* OnUpdate(UpdateCallback cb_);
        Animator* OnComplete(EndCallback cb_);
        float GetValue();
        bool IsValid();
        void Forward();
        void Reverse();
        void Stop();
        void Dispose();
    };
}
} // namespace ant2d

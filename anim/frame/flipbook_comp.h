#pragma once
#include <cstdint>
#include <gfx/i_comp.h>
#include <string>

namespace ant2d {
namespace frame {

    class FlipbookComp : public IComp {
    public:
        using LoopType = uint8_t;
        static constexpr LoopType kRestart = 0;
        static constexpr LoopType kPingPong = 1;
        static constexpr LoopType kNone = 2;

        std::string define_;
        float dt_;
        float rate_;
        int ii_;
        bool running_;
        bool loop_;
        uint16_t frame_index_;
        uint16_t last_frame_index_;
        int gfi_;
        LoopType typ_;
        bool reverse_;

    public:
        void Reset()
        {
        }
        void Play(const std::string& name);
        void Resume();
        void Pause();
        void Stop();
        bool IsRunning();
        void SetAnimation(const std::string& name);
        std::tuple<bool, LoopType> Loop();
        void SetLoop(bool v, LoopType typ);
        float Rate();
        void SetRate(float r);
        std::tuple<uint16_t, uint16_t> FrameIndex();
    };
}
} // namespace ant2d

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

        std::string define_ = "";
        float dt_ = 0.0f;
        float rate_ = 0.0f;
        int ii_ = 0;
        bool running_ = false;
        bool loop_ = false;
        uint16_t frame_index_ = 0;
        uint16_t last_frame_index_ = 0;
        int gfi_ = 0;
        LoopType typ_ = kRestart;
        bool reverse_ = false;

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

#include <anim/frame/flipbook_comp.h>

namespace ant2d {
namespace frame {

    void FlipbookComp::Play(const std::string& name)
    {
        define_ = name;
        running_ = true;
        reverse_ = false;
        frame_index_ = 0;
        last_frame_index_ = 0;
        dt_ = 0;
        ii_ = 0;
    }

    void FlipbookComp::Resume()
    {
        running_ = true;
    }

    void FlipbookComp::Pause()
    {
        running_ = false;
    }

    void FlipbookComp::Stop()
    {
        running_ = false;
    }

    bool FlipbookComp::IsRunning()
    {
        return running_;
    }

    void FlipbookComp::SetAnimation(const std::string& name)
    {
        define_ = name;
    }

    std::tuple<bool, FlipbookComp::LoopType> FlipbookComp::Loop()
    {
        return { loop_, typ_ };
    }

    void FlipbookComp::SetLoop(bool v, LoopType typ)
    {
        loop_ = v;
        typ_ = typ;
    }

    float FlipbookComp::Rate()
    {
        return rate_;
    }

    void FlipbookComp::SetRate(float r)
    {
        rate_ = r;
    }

    std::tuple<uint16_t, uint16_t> FlipbookComp::FrameIndex()
    {
        return { frame_index_, last_frame_index_ };
    }

}
} // namespace ant2d

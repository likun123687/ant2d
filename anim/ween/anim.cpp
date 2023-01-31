#include <anim/ween/anim.h>
#include <utils/debug.h>

namespace ant2d {
namespace ween {
    void Animation::Reset()
    {
        interpolator = math::ease::Linear;
        state.anim_state = kWaiting;
        clock = 0;
        loop_type = kRestart;
        repeat_count = 0;
        reverse = false;
    }

    // 动画核心算法
    float Animation::Animate(float dt_)
    {
        float ret = 0;
        clock += dt_;
        auto fr = clock / duration;
        if (fr >= 1) {
            if (iteration < repeat_count || repeat_count == kRepeatInfinite) {
                iteration += int(fr);
                clock = 0;
                if (loop_type == kPingPong) {
                    reverse = !reverse;
                }

                while (fr >= 1) {
                    fr -= 1;
                }

            } else {
                state.anim_state = kStopped;
                state.dirty = true;
                fr = 1;
            }
        }

        if (state.anim_state == kStopped) {
            ret = 1;
        } else {
            ret = float(interpolator(double(fr)));
        }

        if (reverse) {
            ret = 1 - ret;
        }

        return ret;
    }

    Animation::Animation()
        : index { 0 }
        , clock { 0.0f }
        , duration { 0.0f }
        , iteration { 0 }
        , repeat_count { 0 }
        , loop_type { kRestart }
    {
    }

    TweenEngine::TweenEngine()
        : anims {}
        , values {}
        , callbacks {}
        , time { 0.0f }
        , scale { 0.0f }
        , active { 0 }
        , cap { 0 }
        , loopup {}
        , unique_id { 0 }
    {
        scale = 1;
        anims.resize(32);
        values.resize(32);
        callbacks.resize(32);
    }

    int TweenEngine::New()
    {
        int uid = unique_id;
        unique_id++;

        auto index = active;
        active++;

        auto& anim = anims[index];
        anim.Reset();
        anim.index = uid;
        values[index] = Value {};
        callbacks[index] = Callback {};
        loopup[uid] = index;
        return uid;
    }

    // Animator TweenEngine::NewAnimator()
    // {
    //     return Animator { this, New() };
    // }

    void TweenEngine::Delete(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            anims[iter->second].state.anim_state = kDispose;
        }
    }

    void TweenEngine::SetTimeScale(float sk_)
    {
        scale = sk_;
    }

    void TweenEngine::Update(float dt_)
    {
        //  1. update
        for (int i = 0; i < active; i++) {
            auto& anim = anims[i];
            if (anim.state.anim_state == kRunning) {
                auto f = anim.Animate(dt_);
                values[i] = Value { f };
            }
        }

        // 2 callback
        for (int i = 0; i < active; i++) {
            auto& anim = anims[i];
            if (anim.state.anim_state == kStopped && anim.state.dirty) {
                anim.state.dirty = false;
                auto cb = callbacks[i].end_callback;
                if (cb) {
                    cb(anim.reverse);
                }
            } else if (anim.state.anim_state == kRunning) {
                auto cb = callbacks[i].update_callback;
                if (cb) {
                    cb(anim.reverse, values[i].f);
                }
            }
        }

        // 3. delete dead
        int i = 0, j = active - 1;
        while (i <= j) {
            auto& anim = anims[i];
            if (anim.state.anim_state == kDispose) {
                loopup[anims[j].index] = i;
                loopup.erase(anim.index);
                anims[i] = anims[j];
                values[i] = values[j];
                callbacks[i] = callbacks[j];
                j--;
            } else {
                i++;
            }
        }

        active = i;
    }

    void TweenEngine::Forward(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            auto& anim = anims[iter->second];
            anim.clock = 0;
            anim.state.anim_state = kRunning;
            anim.state.dirty = true;
            anim.iteration = 0;
            anim.reverse = false;

            values[iter->second] = Value {};
        }
    }

    void TweenEngine::Reverse(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            auto v = iter->second;
            auto& anim = anims[v];
            if (anim.state.anim_state == kRunning) {
                anim.clock = anim.duration - anim.clock;
                anim.reverse = !anim.reverse;
            } else {
                anim.reverse = !anim.reverse;
                anim.clock = 0;
                anim.state.anim_state = kRunning;
                anim.state.dirty = true;
                anim.iteration = 0;
            }
        }
    }

    // Stops running this animation.
    void TweenEngine::Stop(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            auto v = iter->second;
            auto& anim = anims[v];
            anim.state.anim_state = kStopped;
            anim.state.dirty = true;
        }
    }

    // Duration is the length of time this animation should last.
    void TweenEngine::SetDuration(int index_, float d_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            anims[iter->second].duration = d_;
        }
    }

    // Repeat the animation. If playback type is forward, restart the animation
    // from start, if the playback type is backward or ping-pong,
    void TweenEngine::SetRepeat(int index_, int count_, LoopType loop_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            auto v = iter->second;
            auto& anim = anims[v];
            anim.repeat_count = count_;
            anim.loop_type = loop_;
        }
    }

    void TweenEngine::SetFunction(int index_, math::ease::Function fn_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            auto v = iter->second;
            if (fn_) {
                anims[v].interpolator = fn_;
            } else {
                anims[v].interpolator = math::ease::Linear;
            }
        }
    }

    void TweenEngine::SetUpdateCallback(int index_, UpdateCallback cb_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            callbacks[iter->second].update_callback = cb_;
        }
    }

    void TweenEngine::SetCompleteCallback(int index_, EndCallback cb_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            callbacks[iter->second].end_callback = cb_;
        }
    }

    float TweenEngine::GetValue(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            return values[iter->second].f;
        }
        return 0;
    }

    float TweenEngine::GetDuration(int index_)
    {
        auto iter = loopup.find(index_);
        if (iter != loopup.end()) {
            return anims[iter->second].duration;
        }
        return 0;
    }
}
} // namespace ant2d

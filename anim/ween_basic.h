#pragma once
#include <anim/ween/animator.h>
#include <anim/ween/tweens.h>
#include <anim/ween/anim.h>

namespace ant2d {
namespace anim {
    // A fire-and-forget pattern Animator.
    struct ProxyAnimator {
        ween::Animator animator;
        ween::UpdateCallback update;
        ween::EndCallback complete;

        //ProxyAnimator();
        ProxyAnimator(ween::TweenEngine* en_);

        // Proxy the SetDuration method.
        ProxyAnimator* SetDuration(float d_);

        // Proxy the SetRepeat method.
        ProxyAnimator* SetRepeat(int count_, ween::LoopType loop_);

        // Proxy the SetFunction method.
        ProxyAnimator* SetFunction(math::ease::Function function_);

        // Proxy the OnUpdate method. proxyAnimator uses the UpdateCallback to update values internally,
        // the user UpdateCallback will be called after it.
        ProxyAnimator* OnUpdate(ween::UpdateCallback fn_);

        // Proxy the OnComplete method. proxyAnimator uses the CompleteCallback to remove itself from
        // the TweenEngine, the user CompleteCallback will be called after it.
        ProxyAnimator* OnUpdaOnCompletete(ween::EndCallback fn_);
    };

    // OfFloat returns a Animator that animates between float values.
    std::unique_ptr<ProxyAnimator> OfFloat(float* target, float from, float to);

    // OfVec2 returns a Animator that animates between f32.Vec2 values.
    std::unique_ptr<ProxyAnimator> OfVec2(math::Vec2* target, math::Vec2 from, math::Vec2 to);

    // OfColor returns a Animator that animates between gfx.Color values.
    std::unique_ptr<ProxyAnimator> OfColor(color::Float4* target, color::Float4 from, color::Float4 to);
}
} // namespace ant2d

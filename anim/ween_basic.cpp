#include <anim/ween_basic.h>
#include <anim/anim.h>

namespace ant2d {
namespace anim {
    // ProxyAnimator::ProxyAnimator()
    //     : animator {}
    // {
    // }

    ProxyAnimator::ProxyAnimator(ween::TweenEngine* en_)
        : animator { en_, en_->New() }
        , update { nullptr }
        , complete { nullptr }
    {
    }

    // Proxy the SetDuration method.
    ProxyAnimator* ProxyAnimator::SetDuration(float d_)
    {
        animator.SetDuration(d_);
        return this;
    }

    // Proxy the SetRepeat method.
    ProxyAnimator* ProxyAnimator::SetRepeat(int count_, ween::LoopType loop_)
    {
        animator.SetRepeat(count_, loop_);
        return this;
    }

    // Proxy the SetFunction method.
    ProxyAnimator* ProxyAnimator::SetFunction(math::ease::Function function_)
    {
        animator.SetFunction(function_);
        return this;
    }

    // Proxy the OnUpdate method. proxyAnimator uses the UpdateCallback to update values internally,
    // the user UpdateCallback will be called after it.
    ProxyAnimator* ProxyAnimator::OnUpdate(ween::UpdateCallback fn_)
    {
        update = fn_;
        return this;
    }

    // Proxy the OnComplete method. proxyAnimator uses the CompleteCallback to remove itself from
    // the TweenEngine, the user CompleteCallback will be called after it.
    ProxyAnimator* ProxyAnimator::OnUpdaOnCompletete(ween::EndCallback fn_)
    {
        complete = fn_;
        return this;
    }

    // OfFloat returns a Animator that animates between float values.
    std::unique_ptr<ProxyAnimator> OfFloat(float* target, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator(SharedTweenEngine);
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [target, from, to, p_animator](bool r, float f) -> void {
            *target = ween::F32Lerp(from, to, f);
            auto fn = p_animator->update;
            if (fn) {
                fn(r, f);
            }
        };

        auto complete_cb = [p_animator](bool reverse) -> void {
            p_animator->animator.Dispose();
            auto fn = p_animator->complete;
            if (fn) {
                fn(reverse);
            }
        };

        proxy->animator.OnUpdate(update_cb);
        proxy->animator.OnComplete(complete_cb);
        return std::move(proxy);
    }

    // OfVec2 returns a Animator that animates between f32.Vec2 values.
    std::unique_ptr<ProxyAnimator> OfVec2(math::Vec2* target, math::Vec2 from, math::Vec2 to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator(SharedTweenEngine);
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [target, from, to, p_animator](bool r, float f) -> void {
            *target = ween::Vec2Lerp(from, to, f);
            auto fn = p_animator->update;
            if (fn) {
                fn(r, f);
            }
        };

        auto complete_cb = [p_animator](bool reverse) -> void {
            p_animator->animator.Dispose();
            auto fn = p_animator->complete;
            if (fn) {
                fn(reverse);
            }
        };

        proxy->animator.OnUpdate(update_cb);
        proxy->animator.OnComplete(complete_cb);
        return std::move(proxy);
    }

    // OfColor returns a Animator that animates between gfx.Color values.
    std::unique_ptr<ProxyAnimator> OfColor(color::Float4* target, color::Float4 from, color::Float4 to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator(SharedTweenEngine);
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [target, from, to, p_animator](bool r, float f) -> void {
            *target = ween::ColorLerp(from, to, f);
            auto fn = p_animator->update;
            if (fn) {
                fn(r, f);
            }
        };

        auto complete_cb = [p_animator](bool reverse) -> void {
            p_animator->animator.Dispose();
            auto fn = p_animator->complete;
            if (fn) {
                fn(reverse);
            }
        };

        proxy->animator.OnUpdate(update_cb);
        proxy->animator.OnComplete(complete_cb);
        return std::move(proxy);
    }
}
} // namespace ant2d

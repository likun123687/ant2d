#include <anim/ween_transform.h>

namespace ant2d {
namespace anim {
    // Convenient methods that uses to animate the Transform Component.

    // Move the Entity to given value.
    std::unique_ptr<ProxyAnimator> Move(Entity e, math::Vec2 from, math::Vec2 to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                comp->SetPosition(ween::Vec2Lerp(from, to, f));
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Move the 'x' to given value.
    std::unique_ptr<ProxyAnimator> MoveX(Entity e, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto x = ween::F32Lerp(from, to, f);
                auto old_pos = comp->GetPosition();
                comp->SetPosition(math::Vec2 { x, old_pos[1] });
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Move the 'y' to given value.
    std::unique_ptr<ProxyAnimator> MoveY(Entity e, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto y = ween::F32Lerp(from, to, f);
                auto old_pos = comp->GetPosition();
                comp->SetPosition(math::Vec2 { old_pos[0], y });
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Scale the Entity to the given value.
    std::unique_ptr<ProxyAnimator> Scale(Entity e, math::Vec2 from, math::Vec2 to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto scale = ween::Vec2Lerp(from, to, f);
                comp->SetScale(scale);
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Scale the 'x' to the given value.
    std::unique_ptr<ProxyAnimator> ScaleX(Entity e, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto x = ween::F32Lerp(from, to, f);
                auto old_scale = comp->GetScale();
                comp->SetScale(math::Vec2 { x, old_scale[1] });
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Scale the 'y' to the given value.
    std::unique_ptr<ProxyAnimator> ScaleY(Entity e, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto y = ween::F32Lerp(from, to, f);
                auto old_scale = comp->GetScale();
                comp->SetScale(math::Vec2 { old_scale[0], y });
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

    // Rotate the Entity to given value.
    std::unique_ptr<ProxyAnimator> Rotate(Entity e, float from, float to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto comp = SharedAnimationSystem->transform_table->GetComp(e);
            if (comp) {
                auto rotate = ween::F32Lerp(from, to, f);
                comp->SetRotation(ween::F32Lerp(from, to, f));
            }
            auto fn = p_animator->update;
            if (fn) {
                fn(reverse, f);
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

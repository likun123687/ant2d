#include <anim/ween_sprite.h>

namespace ant2d {
namespace anim {
    // Convenient methods that uses to animate the Sprite Component.
    // Tint the Entity to given color.
    std::unique_ptr<ProxyAnimator> Tint(Entity e, color::Float4 from, color::Float4 to)
    {
        ProxyAnimator* p_animator = new ProxyAnimator();
        std::unique_ptr<ProxyAnimator> proxy { p_animator };
        auto update_cb = [e, from, to, p_animator](bool reverse, float f) -> void {
            auto spr = SharedAnimationSystem->sprite_table->GetComp(e);
            if (spr) {
                auto c = ween::ColorLerp(from, to, f);
                spr->SetColor(c.GetByte4Color());
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
}
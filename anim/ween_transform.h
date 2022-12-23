#pragma once

#include <anim/ween_basic.h>
#include <anim/anim.h>
namespace ant2d {
namespace anim {
    // Convenient methods that uses to animate the Transform Component.

    // Move the Entity to given value.
    std::unique_ptr<ProxyAnimator> Move(Entity e, math::Vec2 from, math::Vec2 to);

    // Move the 'x' to given value.
    std::unique_ptr<ProxyAnimator> MoveX(Entity e, float from, float to);

    // Move the 'y' to given value.
    std::unique_ptr<ProxyAnimator> MoveY(Entity e, float from, float to);

    // Scale the Entity to the given value.
    std::unique_ptr<ProxyAnimator> Scale(Entity e, math::Vec2 from, math::Vec2 to);

    // Scale the 'x' to the given value.
    std::unique_ptr<ProxyAnimator> ScaleX(Entity e, float from, float to);

    // Scale the 'y' to the given value.
    std::unique_ptr<ProxyAnimator> ScaleY(Entity e, float from, float to);

    // Rotate the Entity to given value.
    std::unique_ptr<ProxyAnimator> Rotate(Entity e, float from, float to);

}
} // namespace ant2d

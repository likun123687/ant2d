#pragma once
#include <anim/ween_basic.h>
#include <engi/entity.h>
#include <anim/anim.h>

namespace ant2d {
namespace anim {
    // Convenient methods that uses to animate the Sprite Component.
    // Tint the Entity to given color.
    std::unique_ptr<ProxyAnimator> Tint(Entity e, color::Byte4 from, color::Byte4 to);
}
}
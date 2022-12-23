#pragma once
#include <anim/frame/anim.h>
#include <anim/ween/anim.h>
#include <gfx/transform/transform_table.h>

namespace ant2d {
namespace anim {
    extern frame::SpriteEngine* SharedSpriteEngine;
    extern ween::TweenEngine* SharedTweenEngine;
    struct AnimationSystem;
    extern AnimationSystem* SharedAnimationSystem;

    struct AnimationSystem {
        std::unique_ptr<frame::SpriteEngine> sprite_engine;
        std::unique_ptr<ween::TweenEngine> tween_engine;

        SpriteTable* sprite_table;
        TransformTable* transform_table;
        AnimationSystem();
        void RequireTable(TableList* table_list_);
        void Update(float dt_);
    };

    void SetDefaultAnimationSystem(AnimationSystem* as);
}
} // namespace ant2d

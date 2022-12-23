#include <anim/anim.h>

namespace ant2d {
namespace anim {
    frame::SpriteEngine* SharedSpriteEngine = nullptr;
    ween::TweenEngine* SharedTweenEngine = nullptr;
    AnimationSystem* SharedAnimationSystem = nullptr;

    AnimationSystem::AnimationSystem()
        : sprite_engine(new frame::SpriteEngine())
        , tween_engine(new ween::TweenEngine())
    {
    }

    void AnimationSystem::RequireTable(TableList* table_list_)
    {
        sprite_engine->RequireTable(table_list_);
        for (auto& t : *table_list_) {
            if (t->GetTableType() == TableType::kSprite) {
                sprite_table = reinterpret_cast<SpriteTable*>(t.get());
            } else if (t->GetTableType() == TableType::kTransform) {
                transform_table = reinterpret_cast<TransformTable*>(t.get());
            }
        }
    }

    void AnimationSystem::Update(float dt_)
    {
        sprite_engine->Update(dt_);
        tween_engine->Update(dt_);
    }

    // set shortcut
    void SetDefaultAnimationSystem(AnimationSystem* as)
    {
        SharedAnimationSystem = as;
        SharedSpriteEngine = as->sprite_engine.get();
        SharedTweenEngine = as->tween_engine.get();
    }
}
} // namespace ant2d

#include <ant2d.h>
#include <math/ease/elastic.h>
#include <math/ease/ease.h>
#include <math/ease/circle.h>
#include <math/ease/bounce.h>
#include <math/ease/back.h>
#include <math/ease/cubic.h>

#include <anim/ween_transform.h>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

class MainScene : public ant2d::Scene {
    ant2d::Entity hero_;
    std::vector<std::unique_ptr<ant2d::anim::ProxyAnimator>> anim_list_;

    void OnEnter(ant2d::Game* g)
    {
        // texture
        auto tex = ant2d::SharedTextureManager->Get("assets/block.png");
        // ease functions
        std::vector<ant2d::math::ease::Function> funcs {
            ant2d::math::ease::Linear,
            ant2d::math::ease::OutCirc,
            ant2d::math::ease::OutBounce,
            ant2d::math::ease::OutElastic,
            ant2d::math::ease::OutBack,
            ant2d::math::ease::OutCubic,
        };

        for (int i = 0; i < funcs.size(); i++) {
            auto entity = ant2d::SharedEntityManager->New();
            ant2d::SharedSpriteTable->NewCompX(entity, tex)->SetSize(100, 100);
            ant2d::SharedTransformTable->NewComp(entity)->SetPosition(ant2d::math::Vec2 { 0, 50 + 30 * float(i) });
            anim_list_.emplace_back(ant2d::anim::MoveX(entity, 10, 540));
            anim_list_.back()->SetFunction(funcs[i])->SetRepeat(ant2d::ween::kRepeatInfinite, ant2d::ween::kRestart)->SetDuration(2)->animator.Forward();
        }
    }

    void Update(float dt)
    {
    }

    void OnExit()
    {
    }
};

ant2d::WindowOptions* ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        ant2d::SharedTextureManager->Load("assets/block.png");
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "tween anim test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

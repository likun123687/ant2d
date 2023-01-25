#include <ant2d.h>
#include <iostream>

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
    void OnEnter(ant2d::Game* g)
    {
        // get animation system...
        // input control
        ant2d::SharedInputSystem->RegisterButton("up", { SAPP_KEYCODE_UP });
        ant2d::SharedInputSystem->RegisterButton("down", { SAPP_KEYCODE_DOWN });
        ant2d::SharedInputSystem->RegisterButton("left", { SAPP_KEYCODE_LEFT });
        ant2d::SharedInputSystem->RegisterButton("right", { SAPP_KEYCODE_RIGHT });

        hero_ = ant2d::SharedEntityManager->New();
        // SpriteComp
        ant2d::SharedSpriteTable->NewComp(hero_)->SetSize(50, 50);
        ant2d::SharedTransformTable->NewComp(hero_)->SetPosition(ant2d::math::Vec2 { 240, 160 });

        auto fb = ant2d::SharedFlipbookTable->NewComp(hero_);
        fb->SetRate(0.2);
        fb->SetLoop(true, ant2d::frame::FlipbookComp::kRestart);

        auto at = ant2d::SharedTextureManager->GetAtlas("assets/hero.png");
        if (!at) {
            Error("get atlas error");
        }

        std::vector<ant2d::ITexture2D*> frames(12, nullptr);
        for (int i = 0; i < 12; i++) {
            frames[i] = at->GetSubTexByIndex(i);
        }
        ant2d::anim::SharedSpriteEngine->NewAnimation("hero.down", frames.begin(), 3, true);
        ant2d::anim::SharedSpriteEngine->NewAnimation("hero.left", frames.begin() + 3, 3, true);
        ant2d::anim::SharedSpriteEngine->NewAnimation("hero.right", frames.begin() + 6, 3, true);
        ant2d::anim::SharedSpriteEngine->NewAnimation("hero.top", frames.begin() + 9, 3, true);

        fb->Play("hero.down");
    }

    void Update(float dt)
    {
        ant2d::math::Vec2 speed { 0, 0 };
        if (ant2d::SharedInputSystem->GetButton("up")->JustPressed()) {
            ant2d::SharedFlipbookTable->GetComp(hero_)->Play("hero.top");
        }

        if (ant2d::SharedInputSystem->GetButton("down")->JustPressed()) {
            ant2d::SharedFlipbookTable->GetComp(hero_)->Play("hero.down");
        }

        if (ant2d::SharedInputSystem->GetButton("left")->JustPressed()) {
            ant2d::SharedFlipbookTable->GetComp(hero_)->Play("hero.left");
        }
        if (ant2d::SharedInputSystem->GetButton("right")->JustPressed()) {
            ant2d::SharedFlipbookTable->GetComp(hero_)->Play("hero.right");
        }

        float scalar = 3;
        if (ant2d::SharedInputSystem->GetButton("up")->Down()) {
            speed[1] = scalar;
        }

        if (ant2d::SharedInputSystem->GetButton("down")->Down()) {
            speed[1] = -scalar;
        }

        if (ant2d::SharedInputSystem->GetButton("left")->Down()) {
            speed[0] = -scalar;
        }

        if (ant2d::SharedInputSystem->GetButton("right")->Down()) {
            speed[0] = scalar;
        }

        auto xf = ant2d::SharedTransformTable->GetComp(hero_);

        auto x = xf->GetPosition()[0] + speed[0];
        auto y = xf->GetPosition()[1] + speed[1];

        xf->SetPosition(ant2d::math::Vec2 { x, y });
    }

    void OnExit()
    {
    }
};

ant2d::WindowOptions* ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        ant2d::SharedTextureManager->LoadAtlasIndexed("assets/hero.png", 52, 72, 4, 3);
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "sprite anim test", 800, 600 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

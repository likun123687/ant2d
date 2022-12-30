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
    ant2d::Entity face_;
    void OnEnter(ant2d::Game* g)
    {
        Info("main scene on enter");
        auto tex = ant2d::SharedTextureManager->Get("assets/face.png");
        auto face = ant2d::SharedEntityManager->New();
        auto sprite = ant2d::SharedSpriteTable->NewCompX(face, tex);
        sprite->SetSize(50, 50);

        auto trans = ant2d::SharedTransformTable->NewComp(face);
        trans->SetPosition(ant2d::math::Vec2 { 100, 100 });

        face_ = face;
    }

    void Update(float dt)
    {
        Info("main scene update {}", dt);
        ant2d::math::Vec2 speed { 0, 0 };
        if (ant2d::SharedInputSystem->GetButton("up")->Down()) {
            Info("up button clicked");
            speed[1] = 5;
        }

        if (ant2d::SharedInputSystem->GetButton("down")->Down()) {
            speed[1] = -5;
        }

        if (ant2d::SharedInputSystem->GetButton("left")->Down()) {
            speed[0] = -5;
        }

        if (ant2d::SharedInputSystem->GetButton("right")->Down()) {
            speed[0] = 5;
        }

        auto xf = ant2d::SharedTransformTable->GetComp(face_);
        xf->MoveBy(speed[0], speed[1]);
    }

    void OnExit()
    {
    }
};

ant2d::WindowOptions ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        ant2d::SharedTextureManager->Load("assets/face.png");
        ant2d::SharedInputSystem->RegisterButton("up", { SAPP_KEYCODE_UP });
        ant2d::SharedInputSystem->RegisterButton("down", { SAPP_KEYCODE_DOWN });
        ant2d::SharedInputSystem->RegisterButton("left", { SAPP_KEYCODE_LEFT });
        ant2d::SharedInputSystem->RegisterButton("right", { SAPP_KEYCODE_RIGHT });
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = ant2d::WindowOptions { "input test", 800, 600 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

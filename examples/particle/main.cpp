#include <ant2d.h>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

class MainScene : public ant2d::Scene {
    void OnEnter(ant2d::Game* g)
    {
        Info("main scene on enter");
        auto cfg = ant2d::GravityConfig {};
        cfg.max = 1024;
        cfg.rate = 10;
        cfg.duration = ant2d::math::MaxFloat32;
        cfg.life = ant2d::Var { 40.1, 0.4 };
        cfg.size = ant2d::Range { ant2d::Var { 10, 5 }, ant2d::Var { 20, 5 } };
        cfg.x = ant2d::Var { 0, 0 };
        cfg.y = ant2d::Var { 0, 0 };
        cfg.a = ant2d::Range { ant2d::Var { 1, 0 }, ant2d::Var { 0, 0 } };
        cfg.speed = ant2d::Var { 70, 10 };
        cfg.angel = ant2d::Var { ant2d::math::Radian(90), ant2d::math::Radian(30) };
        cfg.gravity = ant2d::math::Vec2 { 0, -10 };

        auto entity = ant2d::SharedEntityManager->New();
        auto particle = ant2d::SharedParticleSystemTable->NewComp(entity);
        particle->SetSimulator(new ant2d::GravitySimulator(cfg));
        auto tex = ant2d::SharedTextureManager->Get("assets/particle.png");
        particle->SetTexture(tex);
        auto xf = ant2d::SharedTransformTable->NewComp(entity);
        xf->SetPosition(ant2d::math::Vec2 { 100, 100 });
    }

    void Update(float dt)
    {
        Info("main scene update {}", dt);
    }

    void OnExit()
    {
    }
};

ant2d::WindowOptions ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        ant2d::SharedTextureManager->Load("assets/particle.png");
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = ant2d::WindowOptions { "Mesh test", 800, 600 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

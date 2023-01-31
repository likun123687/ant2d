#include <ant2d.h>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

struct SpinObject : public ant2d::Script {
    ant2d::Entity entity;
    float angle;
    SpinObject()
    {
        entity = ant2d::SharedEntityManager->New();
        angle = 0.0f;
        ant2d::SharedSpriteTable->NewComp(entity);
        ant2d::SharedTransformTable->NewComp(entity);
        ant2d::SharedScriptTable->NewComp(entity, this);
    }

    void Init()
    {
    }

    void Update(float dt)
    {
        auto an = dt * 240 / 360 * 6.28;
        auto a = angle + an;
        angle = a;
        auto dx = ant2d::math::Cos(a) * 60;
        auto dy = ant2d::math::Sin(a) * 60;
        SetPosition(float(240 + dx), float(160 + dy));
    }

    void Destroy()
    {
    }

    void SetTexture(ant2d::ITexture2D* tex)
    {
        auto comp = ant2d::SharedSpriteTable->GetComp(entity);
        if (comp) {
            comp->SetSprite(tex);
        }
    }

    void SetSize(float w, float h)
    {
        auto comp = ant2d::SharedSpriteTable->GetComp(entity);
        if (comp) {
            comp->SetSize(w, h);
        }
    }

    void SetPosition(float x, float y)
    {
        auto comp = ant2d::SharedTransformTable->GetComp(entity);
        if (comp) {
            comp->SetPosition(ant2d::math::Vec2 { x, y });
        }
    }
};

class MainScene : public ant2d::Scene {
    std::unique_ptr<SpinObject> spin_;
    void OnEnter(ant2d::Game* g)
    {
        spin_ = std::make_unique<SpinObject>();
        auto tex = ant2d::SharedTextureManager->Get("assets/face.png");
        spin_->SetTexture(tex);
        spin_->SetSize(50, 50);
        spin_->SetPosition(100, 100);
    }

    void Update(float dt)
    {
    }

    void OnExit()
    {
        spin_.reset();
    }
};

ant2d::WindowOptions* ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        Info("main scene on load callback");
        ant2d::SharedTextureManager->Load("assets/face.png");
        ant2d::SharedTextureManager->Load("assets/block.png");
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "script test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

#include <ant2d.h>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

struct Block {
    ant2d::Entity entity;
    Block()
    {
        entity = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewComp(entity);
        ant2d::SharedTransformTable->NewComp(entity);
    }

    void SetTexture(ant2d::ITexture2D* tex)
    {
        ant2d::SharedSpriteTable->GetComp(entity)->SetSprite(tex);
    }

    void SetPosition(float x, float y)
    {
        ant2d::SharedTransformTable->GetComp(entity)->SetPosition(ant2d::math::Vec2 { x, y });
    }

    void SetSize(float w, float h)
    {
        ant2d::SharedSpriteTable->GetComp(entity)->SetSize(w, h);
    }
};

struct Face {
    ant2d::Entity entity;
    Block up;
    Block down;
    Block left;
    Block right;

    Face()
    {
        entity = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewComp(entity);
        ant2d::SharedTransformTable->NewComp(entity);
    }

    void SetTexture(ant2d::ITexture2D* tex)
    {
        ant2d::SharedSpriteTable->GetComp(entity)->SetSprite(tex);
    }

    void SetPosition(float x, float y)
    {
        ant2d::SharedTransformTable->GetComp(entity)->SetPosition(ant2d::math::Vec2 { x, y });
    }

    void SetSize(float w, float h)
    {
        ant2d::SharedSpriteTable->GetComp(entity)->SetSize(w, h);
    }

    void LoadBlock(Block up, Block down, Block left, Block right)
    {
        auto xf = ant2d::SharedTransformTable->GetComp(entity);
        auto b1 = ant2d::SharedTransformTable->GetComp(up.entity);
        auto b2 = ant2d::SharedTransformTable->GetComp(down.entity);
        auto b3 = ant2d::SharedTransformTable->GetComp(left.entity);
        auto b4 = ant2d::SharedTransformTable->GetComp(right.entity);

        xf->LinkChildren({ b1, b2, b3, b4 });
        b1->SetPosition(ant2d::math::Vec2 { 0, 100 });
        b2->SetPosition(ant2d::math::Vec2 { 0, -100 });
        b3->SetPosition(ant2d::math::Vec2 { -100, 0 });
        b4->SetPosition(ant2d::math::Vec2 { 100, 0 });
    }
};

class MainScene : public ant2d::Scene {
    std::unique_ptr<Face> face_;
    void OnEnter(ant2d::Game* g)
    {
        auto block_tex = ant2d::SharedTextureManager->Get("assets/block.png");
        Block up {};
        Block down {};
        Block left {};
        Block right {};

        up.SetTexture(block_tex);
        up.SetSize(100, 100);

        down.SetTexture(block_tex);
        down.SetSize(100, 100);

        left.SetTexture(block_tex);
        left.SetSize(100, 100);

        right.SetTexture(block_tex);
        right.SetSize(100, 100);

        auto face_tex = ant2d::SharedTextureManager->Get("assets/face.png");
        face_ = std::make_unique<Face>();
        face_->SetTexture(face_tex);
        face_->LoadBlock(up, down, left, right);
        face_->SetPosition(240, 160);
        face_->SetSize(100, 100);
    }

    void Update(float dt)
    {
        if (dt > 1) {
            return;
        }

        float x = 0;
        float y = 0;

        if (ant2d::SharedInputSystem->GetButton("up")->Down()) {
            y = 100;
        }

        if (ant2d::SharedInputSystem->GetButton("down")->Down()) {
            y = -100;
        }

        if (ant2d::SharedInputSystem->GetButton("left")->Down()) {
            x = -100;
        }

        if (ant2d::SharedInputSystem->GetButton("right")->Down()) {
            x = 100;
        }

        auto xf = ant2d::SharedTransformTable->GetComp(face_->entity);
        auto p = xf->GetPosition();
        x = x * dt + p[0];
        y = y * dt + p[1];

        xf->SetPosition(ant2d::math::Vec2 { x, y });
    }

    void OnExit()
    {
        face_.reset();
    }
};

ant2d::WindowOptions ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        Info("main scene on load callback");
        ant2d::SharedTextureManager->Load("assets/face.png");
        ant2d::SharedTextureManager->Load("assets/block.png");

        ant2d::SharedInputSystem->RegisterButton("up", { SAPP_KEYCODE_UP });
        ant2d::SharedInputSystem->RegisterButton("down", { SAPP_KEYCODE_DOWN });
        ant2d::SharedInputSystem->RegisterButton("left", { SAPP_KEYCODE_LEFT });
        ant2d::SharedInputSystem->RegisterButton("right", { SAPP_KEYCODE_RIGHT });
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = ant2d::WindowOptions { "node test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

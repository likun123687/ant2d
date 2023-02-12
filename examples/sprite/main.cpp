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
    void OnEnter(ant2d::Game* g)
    {
        Info("main scene on enter");

        auto tex = ant2d::SharedTextureManager->Get("assets/block.png");
        auto face = ant2d::SharedTextureManager->Get("assets/face.png");
        auto repo = ant2d::SharedTextureManager->GetRepo();
        // assert(repo["assets/face.png"].rid == tex->GetTextureId());

        auto entity = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity, tex)->SetSize(100, 100);
        auto xf = ant2d::SharedTransformTable->NewComp(entity);
        xf->SetPosition(ant2d::math::Vec2 { 200, 200 });

        for (int i = 0; i < 60000; i++) {
            auto entity = ant2d::SharedEntityManager->New();
            ant2d::SharedSpriteTable->NewCompX(entity, tex)->SetSize(40, 40);
            auto xf = ant2d::SharedTransformTable->NewComp(entity);
            float x = ant2d::math::Random(0, 960);
            float y = ant2d::math::Random(0, 400) + 240;
            xf->SetPosition(ant2d::math::Vec2 { x, y });
        }

        // auto entity1 = ant2d::SharedEntityManager->New();
        // ant2d::SharedSpriteTable->NewCompX(entity1, tex)->SetSize(80, 80);
        // auto xf1 = ant2d::SharedTransformTable->NewComp(entity1);
        // xf1->SetPosition(ant2d::math::Vec2 { 200, 200 });

        // auto entity2 = ant2d::SharedEntityManager->New();
        // ant2d::SharedSpriteTable->NewCompX(entity2, tex)->SetSize(80, 80);
        // auto xf2 = ant2d::SharedTransformTable->NewComp(entity2);
        // xf2->SetPosition(ant2d::math::Vec2 { 100, 100 });

        // auto entity_font = ant2d::SharedEntityManager->New();
        // auto tex_font = ant2d::SharedTextureManager->Get("assets/debug_font.png");
        // ant2d::SharedSpriteTable->NewCompX(entity_font, tex_font)->SetSize(999, 54);
        // auto xf_font = ant2d::SharedTransformTable->NewComp(entity_font);
        // xf_font->SetPosition(ant2d::math::Vec2 { 600, 400 });

        // auto tex1 = ant2d::SharedTextureManager->Get("assets/face1.png");
        // Info("tex1 {}", static_cast<void*>(tex1));
        // auto entity3 = ant2d::SharedEntityManager->New();
        // ant2d::SharedSpriteTable->NewCompX(entity3, tex1)->SetSize(80, 80);
        // auto xf3 = ant2d::SharedTransformTable->NewComp(entity3);
        // xf3->SetPosition(ant2d::math::Vec2 { 400, 400 });

        // auto tex2 = ant2d::SharedTextureManager->Get("assets/1111.png");
        // auto entity4 = ant2d::SharedEntityManager->New();
        // ant2d::SharedSpriteTable->NewCompX(entity4, tex2)->SetSize(80, 80);
        // auto xf4 = ant2d::SharedTransformTable->NewComp(entity4);
        // xf4->SetPosition(ant2d::math::Vec2 { 450, 450 });

        // auto atlas = ant2d::SharedTextureManager->GetAtlas("assets/test.png");
        // auto sub_tex = atlas->GetSubTexByName("OBJ_DOOR001.png");

        // auto entity5 = ant2d::SharedEntityManager->New();
        // ant2d::SharedSpriteTable->NewCompX(entity5, sub_tex)->SetSize(80, 160);
        // auto xf5 = ant2d::SharedTransformTable->NewComp(entity5);
        // xf5->SetPosition(ant2d::math::Vec2 { 300, 300 });
    }

    void Update(float dt)
    {
        Info("main scene update {}", dt);
    }

    void OnExit()
    {
    }
};

ant2d::WindowOptions* ant2d_main(int argc, char* argv[])
{
    Info("ant2d main called");
    auto on_load_callback = []() {
        Info("main scene on load callback");
        ant2d::SharedTextureManager->Load("assets/face.png");
        ant2d::SharedTextureManager->Load("assets/face1.png");
        ant2d::SharedTextureManager->Load("assets/1111.png");
        ant2d::SharedTextureManager->Load("assets/block.png");
        ant2d::SharedTextureManager->Load("assets/debug_font.png");
        ant2d::SharedTextureManager->LoadAtlas("assets/test.png", "assets/test.json");

        auto repo = ant2d::SharedTextureManager->GetRepo();
        assert(repo["assets/face.png"].cnt == 1);
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "sprite test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

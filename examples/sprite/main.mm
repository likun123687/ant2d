#include <ant2d.h>

#if BK_PLATFORM_OSX
#import <Foundation/Foundation.h>
#endif

#include <hid/sokol_app_desc.h>
#include <utils/debug.h>

#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_glue.h"

class MainScene:public ant2d::Scene
{
    void OnEnter(ant2d::Game *g)
    {
        Info("main scene on enter");
        auto tex = ant2d::SharedTextureManager->Get("assets/face.png");
        auto repo = ant2d::SharedTextureManager->GetRepo();
        assert(repo["assets/face.png"].rid == tex->GetTextureId());

        auto entity = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity, tex)->SetSize(80, 80);
        auto xf = ant2d::SharedTransformTable->NewComp(entity);
        xf->SetPosition(ant2d::math::Vec2{300, 300});

        auto entity1 = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity1, tex)->SetSize(80, 80);
        auto xf1 = ant2d::SharedTransformTable->NewComp(entity1);
        xf1->SetPosition(ant2d::math::Vec2{50, 50});

        auto entity2 = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity2, tex)->SetSize(80, 80);
        auto xf2 = ant2d::SharedTransformTable->NewComp(entity2);
        xf2->SetPosition(ant2d::math::Vec2{300, 50});

        auto tex1 = ant2d::SharedTextureManager->Get("assets/face1.png");
        Info("tex1 {}", static_cast<void*>(tex1));
        auto entity3 = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity3, tex1)->SetSize(80, 80);
        auto xf3 = ant2d::SharedTransformTable->NewComp(entity3);
        xf3->SetPosition(ant2d::math::Vec2 { 30, 30 });

        auto tex2 = ant2d::SharedTextureManager->Get("assets/1111.png");
        auto entity4 = ant2d::SharedEntityManager->New();
        ant2d::SharedSpriteTable->NewCompX(entity4, tex2)->SetSize(80, 80);
        auto xf4 = ant2d::SharedTransformTable->NewComp(entity4);
        xf4->SetPosition(ant2d::math::Vec2 { 50, 50 });
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
    auto on_load_callback = [](){
        Info("main scene on load callback");
        ant2d::SharedTextureManager->Load("assets/face.png");
        ant2d::SharedTextureManager->Load("assets/face1.png");
        ant2d::SharedTextureManager->Load("assets/1111.png");
        auto repo = ant2d::SharedTextureManager->GetRepo();
        assert(repo["assets/face.png"].cnt == 1);
        Info("face rid {}", repo["assets/face.png"].rid);

        Info("SharedGame--{}, SharedSceneManager--{}, SharedEntityManager--{}, SharedSpriteTable--{}, SharedTransformTable--{}, SharedTextureManager--{}", static_cast<void *>(ant2d::SharedGame), static_cast<void *>(ant2d::SharedSceneManager), static_cast<void *>(ant2d::SharedEntityManager), static_cast<void *>(ant2d::SharedSpriteTable), static_cast<void *>(ant2d::SharedTransformTable), static_cast<void *>(ant2d::SharedTextureManager));
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = ant2d::WindowOptions{"sprite test", 800, 600};
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

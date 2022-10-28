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

        std::vector<uint16_t> s_index = { 3, 1, 2, 3, 2, 0 };

        std::vector<ant2d::PosTexColorVertex> s_vertices = {
            { 200.0, 200.0, 1, 1, 1.0, 1.0, 1.0, 1.0 },
            { 0.0, 0.0, 0, 0, 1.0, 1.0, 1.0, 1.0 },
            { 200.0, 0.0, 1, 0, 1.0, 1.0, 1.0, 1.0 },
            { 0.0, 200.0, 0, 1, 1.0, 1.0, 1.0, 1.0 },
        };

        auto tex = ant2d::SharedTextureManager->Get("assets/face.png");
        auto entity = ant2d::SharedEntityManager->New();
        auto comp = ant2d::SharedMeshTable->NewComp(entity);
        auto mesh = comp->GetMesh();

        mesh->SetIndex(s_index);
        mesh->SetVertex(s_vertices);
        mesh->Setup();
        mesh->SetTexture(tex->GetTextureId());

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
        Info("main scene on load callback");
        ant2d::SharedTextureManager->Load("assets/face.png");
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = ant2d::WindowOptions { "Mesh test", 800, 600 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

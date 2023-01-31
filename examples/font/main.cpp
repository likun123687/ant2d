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
        auto entity = ant2d::SharedEntityManager->New();
        auto font = ant2d::SharedFontManager->Get("font1");

        auto text = ant2d::SharedTextTable->NewComp(entity);
        text->SetFont(font);
        //text->SetColor(0xFF0000FF);

        text->SetText("aaabbbbccc");

        auto xf = ant2d::SharedTransformTable->NewComp(entity);
        xf->SetPosition(ant2d::math::Vec2 { 200, 200 });

        ///////////////////////
        auto entity1 = ant2d::SharedEntityManager->New();
        auto font1 = ant2d::SharedFontManager->Get("font2");
        auto text1 = ant2d::SharedTextTable->NewComp(entity1);
        text1->SetFont(font1);
        //text1->SetColor(0xFF0000FF);
        text1->SetText("aaaaaabbbccc");

        auto xf1 = ant2d::SharedTransformTable->NewComp(entity1);
        xf1->SetPosition(ant2d::math::Vec2 { 300, 300 });
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
        ant2d::SharedFontManager->Loadbitmap("font1", "assets/font.png", "assets/font.json");
        ant2d::SharedFontManager->LoadTrueType("font2", "assets/OCRAEXT.TTF", ant2d::font::truetype::Config { 0, 0, 24 });
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "font test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

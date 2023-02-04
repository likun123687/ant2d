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
    ant2d::ITexture2D* face_ = nullptr;
    float slide_ = 0.0f;
    ant2d::ITexture2D* normal_ = nullptr;
    ant2d::ITexture2D* pressed_ = nullptr;
    bool showbutton_ = false;

    void OnEnter(ant2d::Game* g)
    {
        Info("main scene on enter");
        auto fnt = ant2d::SharedFontManager->Get("font1");
        ant2d::gui::SetFont(fnt);

        // image
        face_ = ant2d::SharedTextureManager->Get("assets/face.png");
        pressed_ = ant2d::SharedTextureManager->Get("assets/press.png");
        normal_ = ant2d::SharedTextureManager->Get("assets/block.png");

        slide_ = 0.5f;
    }

    void Update(float dt)
    {
        Widget();
    }

    void Widget()
    {
        ant2d::gui::Move(100, 60);

        // draw text
        ant2d::gui::Text(2, ant2d::gui::Rect { 0, 0, 0, 0 }, "SomeText", nullptr);
        ant2d::gui::Text(7, ant2d::gui::Rect { 100, 100, 0, 0 }, "SomeTextSome", nullptr);

        ant2d::gui::Image(3, ant2d::gui::Rect { 0, 30, 30, 30 }, face_, nullptr);

        // draw image button
        ant2d::gui::ImageButton(6, ant2d::gui::Rect { 50, 30, 30, 30 }, normal_, pressed_, nullptr);

        // draw button
        auto e = ant2d::gui::Button(4, ant2d::gui::Rect { 0, 100, 0, 0 }, "NewButton", nullptr);
        if ((ant2d::gui::Event::kEventDown & e) != 0) {
            std::cout << "click new button" << std::endl;
            showbutton_ = true;
        }

        if (showbutton_) {
            e = ant2d::gui::Button(5, ant2d::gui::Rect { 0, 150, 0, 0 }, "Dismiss", nullptr);
            if ((ant2d::gui::Event::kEventDown & e) != 0) {
                std::cout << "click new button" << std::endl;
                showbutton_ = false;
            }
        }

        // draw slider
        ant2d::gui::Slider(8, ant2d::gui::Rect { 100, 0, 120, 10 }, &slide_, nullptr);
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
        ant2d::SharedTextureManager->Load("assets/block.png");
        ant2d::SharedTextureManager->Load("assets/particle.png");
        ant2d::SharedTextureManager->Load("assets/press.png");
        ant2d::SharedFontManager->Loadbitmap("font1", "assets/font.png", "assets/font.json");
        ant2d::SharedFontManager->LoadTrueType("font2", "assets/OCRAEXT.TTF", { 0, 0, 24 });
    };
    auto main_scene = new MainScene();
    main_scene->SetOnLoadCallback(on_load_callback);
    auto options = new ant2d::WindowOptions { "gui test", 480, 320 };
    ant2d::Run(options, main_scene);
    Info("ant2d main end");
    return options;
}

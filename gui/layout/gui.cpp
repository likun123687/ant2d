#pragma once
#include <gui/layout/gui.h>

namespace ant2d {
namespace layout {
    // Options: margin
    Options* Margin(float top, float left, float right, float bottom)
    {
        Options* opt = &(LayoutManager::GLayoutManager().options_);
        opt->SetMargin(top, left, right, bottom);
        return opt;
    }

    // Options: gravity
    Options* Gravity(float x, float y)
    {
        Options* opt = &(LayoutManager::GLayoutManager().options_);
        opt->SetGravity(x, y);
        return opt;
    }

    // Options: Size
    Options* Size(float w, float h)
    {
        Options* opt = &(LayoutManager::GLayoutManager().options_);
        opt->SetSize(w, h);
        return opt;
    }

    // Widgets: Text
    void Text(gui::ID id, const std::string& text, gui::TextStyle* style, Options* p)
    {
        if (style == nullptr) {
            style = &(gui::Contex::DefaultContext().GetTheme()->text);
        }

        LayoutManager::GLayoutManager().Text(id, text, style, p);
    }

    void TextSizeColored(gui::ID id, const std::string& text, uint32_t color, float size, Options* p)
    {
        auto style = gui::Contex::DefaultContext().GetTheme()->text;
        style.color = color;
        style.size = size;
        LayoutManager::GLayoutManager().Text(id, text, &style, p);
    }

    // Widgets: InputEditor
    void InputText(const std::string& hint, gui::InputStyle* style, Options* p)
    {
    }

    // Widget: Image
    void Image(gui::ID id, ITexture2D* tex, gui::ImageStyle* style, Options* p)
    {
        if (style == nullptr) {
            style = &(gui::Contex::DefaultContext().GetTheme()->image);
        }
        LayoutManager::GLayoutManager().Image(id, tex, style, p);
    }

    // Widget: Button
    gui::EventType Button(gui::ID id, const std::string& text, gui::ButtonStyle* style, Options* p)
    {
        if (style == nullptr) {
            style = &(gui::Contex::DefaultContext().GetTheme()->button);
        }
        return LayoutManager::GLayoutManager().Button(id, text, style, p);
    }

    gui::EventType ImageButton(gui::ID id, ITexture2D* normal, ITexture2D* pressed, gui::ImageButtonStyle* style, Options* p)
    {
        if (style == nullptr) {
            style = &(gui::Contex::DefaultContext().GetTheme()->image_button);
        }
        return LayoutManager::GLayoutManager().ImageButton(id, normal, pressed, style, p);
    }

    bool CheckBox(const std::string& text, gui::CheckBoxStyle* style)
    {
        return false;
    }

    void ProgressBar(float fraction, gui::ProgressBarStyle* style, Options* p)
    {
    }

    gui::EventType Slider(gui::ID id, float* value, gui::SliderStyle* style, Options* p)
    {
        if (style == nullptr) {
            style = &(gui::Contex::DefaultContext().GetTheme()->slider);
        }
        return LayoutManager::GLayoutManager().Slider(id, value, style, p);
    }

    void ListView()
    {
    }

    // Layout & Group
    // Define a view group
    void Define(const std::string& name)
    {
        return LayoutManager::GLayoutManager().DefineLayout(name, kNormal);
    }

    void DefineType(const std::string& name, ViewType xt)
    {
        return LayoutManager::GLayoutManager().DefineLayout(name, xt);
    }

    void Clear(std::initializer_list<std::string> names)
    {
        return LayoutManager::GLayoutManager().Clear(names);
    }

    void LayoutNormal(gui::ID id, std::function<void(Group*)> gui, float w, float h, LayoutType xt)
    {
        LayoutManager::GLayoutManager().BeginLayout(id, nullptr, xt);
        if (w != 0 || h != 0) {
            LayoutManager::GLayoutManager().current_->SetSize(w, h);
        }
        gui(LayoutManager::GLayoutManager().current_->h_group_);
        LayoutManager::GLayoutManager().EndLayout();
    }

    void LayoutX(gui::ID id, std::function<void(Group*)> gui, Options* opt, LayoutType xt)
    {
        LayoutManager::GLayoutManager().BeginLayout(id, opt, xt);
        if (opt != nullptr) {
            auto w = opt->rect_.w;
            auto h = opt->rect_.h;
            if (w != 0 || h != 0) {
                LayoutManager::GLayoutManager().current_->SetSize(w, h);
            }
            LayoutManager::GLayoutManager().current_->SetGravity(opt->gravity_.x, opt->gravity_.y);
        }
        gui(LayoutManager::GLayoutManager().current_->h_group_);
        LayoutManager::GLayoutManager().EndLayout();
    }

}
} // namespace ant2d

#pragma once
#include <gui/layout/layout_manager.h>
#include <functional>

//	Awesome GUI System
namespace ant2d {
namespace layout {

    // Options: margin
    Options* Margin(float top, float left, float right, float bottom);

    // Options: gravity
    Options* Gravity(float x, float y);

    // Options: Size
    Options* Size(float w, float h);

    // Widgets: Text
    void Text(gui::ID id, const std::string& text, gui::TextStyle* style, Options* p);

    void TextSizeColored(gui::ID id, const std::string& text, color::Byte4 color, float size, Options* p);
    // Widgets: InputEditor
    void InputText(const std::string& hint, gui::InputStyle* style, Options* p);

    // Widget: Image
    void Image(gui::ID id, ITexture2D* tex, gui::ImageStyle* style, Options* p);

    // Widget: Button
    gui::EventType Button(gui::ID id, const std::string& text, gui::ButtonStyle* style, Options* p);

    gui::EventType ImageButton(gui::ID id, ITexture2D* normal, ITexture2D* pressed, gui::ImageButtonStyle* style, Options* p);

    bool CheckBox(const std::string& text, gui::CheckBoxStyle* style);
    void ProgressBar(float fraction, gui::ProgressBarStyle* style, Options* p);
    gui::EventType Slider(gui::ID id, float* value, gui::SliderStyle* style, Options* p);
    void ListView();

    // Layout & Group

    // Define a view group
    void Define(const std::string& name);

    void DefineType(const std::string& name, ViewType xt);

    void Clear(std::initializer_list<std::string> names);

    void LayoutNormal(gui::ID id, std::function<void(Group*)> gui, float w, float h, LayoutType xt);
    void LayoutX(gui::ID id, std::function<void(Group*)> gui, Options* opt, LayoutType xt);

}
} // namespace ant2d

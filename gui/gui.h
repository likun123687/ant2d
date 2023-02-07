#pragma once
#include <gui/id.h>
#include <gui/gui_lib.h>
#include <gui/style.h>

namespace ant2d {
namespace gui {
    void Text(ID id, Rect bb, const std::string& text, TextStyle* style);

    void TextSizeColored(ID id, Rect bb, const std::string& text, color::Byte4 color, float size);
    // Widgets: InputEditor
    void InputText(ID id, Rect bb, const std::string& hint, InputStyle* style);
    // Widget: Image
    void Image(ID id, Rect bb, ITexture2D* tex, ImageStyle* style);
    // Widget: Button
    EventType Button(ID id, Rect bb, const std::string& text, ButtonStyle* style);

    EventType ImageButton(ID id, Rect bb, ITexture2D* normal, ITexture2D* pressed, ImageButtonStyle* style);

    bool CheckBox(ID id, Rect bb, const std::string& text, CheckBoxStyle* style);
    // Widget: ProgressBar, Slider
    void ProgressBar(ID id, Rect bb, float fraction, ProgressBarStyle* style);
    EventType Slider(ID id, Rect bb, float* value, SliderStyle* style);
    // Widget: ColorRect
    void ColorRect(Rect bb, color::Byte4 fill, float rounding);
    // Offset move the ui coordinate's origin by (dx, dy)
    void Offset(float dx, float dy);
    // Move sets the ui coordinate's origin to (x, y)
    void Move(float x, float y);
    // Theme:
    void UseTheme(Theme* style);

    void SetFont(font::FontAtlas* font);

    // Set Z-Order for
    int16_t SetZOrder(int16_t z);

    // for internal usage, DO NOT call.
    void SetScreenSize(float w, float h);
    // ScreenSize return the physical width&height of the screen.
    std::tuple<float, float> ScreenSize();

    std::tuple<float, float> VirtualSize();

    std::tuple<float, float, float, float> HintAndScale();

    // SetVirtualResolution set the virtual resolution.
    void SetVirtualResolution(float w, float h);
    Contex& DefaultContext();

} // namespace gui
} // namespace ant2d

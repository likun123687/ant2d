#include <gui/gui.h>

namespace ant2d {
namespace gui {
    void Text(ID id, Rect bb, const std::string& text, TextStyle* style)
    {
        auto& default_contex = Contex::DefaultContext();
        if (style == nullptr) {
            style = &(default_contex.GetTheme()->text);
        }

        default_contex.Text(id, &bb, text, style);
    }

    void TextSizeColored(ID id, Rect bb, const std::string& text, uint32_t color, float size)
    {
        auto style = Contex::DefaultContext().GetTheme()->text;
        style.color = color;
        style.size = size;
        Contex::DefaultContext().Text(id, &bb, text, &style);
    }

    // Widgets: InputEditor
    void InputText(ID id, Rect bb, const std::string& hint, InputStyle* style)
    {
        Contex::DefaultContext().InputText(id, &bb, hint, style);
    }

    // Widget: Image
    void Image(ID id, Rect bb, ITexture2D* tex, ImageStyle* style)
    {
        Contex::DefaultContext().Image(id, &bb, tex, style);
    }

    // Widget: Button
    EventType Button(ID id, Rect bb, const std::string& text, ButtonStyle* style)
    {
        return Contex::DefaultContext().Button(id, &bb, text, style);
    }

    EventType ImageButton(ID id, Rect bb, ITexture2D* normal, ITexture2D* pressed, ImageButtonStyle* style)
    {
        return Contex::DefaultContext().ImageButton(id, normal, pressed, &bb, style);
    }

    bool CheckBox(ID id, Rect bb, const std::string& text, CheckBoxStyle* style)
    {
        return false;
    }

    // Widget: ProgressBar, Slider
    void ProgressBar(ID id, Rect bb, float fraction, ProgressBarStyle* style)
    {
    }

    EventType Slider(ID id, Rect bb, float* value, SliderStyle* style)
    {
        return Contex::DefaultContext().Slider(id, &bb, value, style);
    }

    // Widget: ColorRect
    void ColorRect(Rect bb, uint32_t fill, float rounding)
    {
        Contex::DefaultContext().DrawRect(&bb, fill, rounding);
    }

    // Offset move the ui coordinate's origin by (dx, dy)
    void Offset(float dx, float dy)
    {
        Contex::DefaultContext().GetCursor().x += dx;
        Contex::DefaultContext().GetCursor().y += dy;
    }

    // Move sets the ui coordinate's origin to (x, y)
    void Move(float x, float y)
    {
        Contex::DefaultContext().GetCursor().x = x;
        Contex::DefaultContext().GetCursor().y = y;
    }

    // Theme:
    void UseTheme(Theme* style)
    {
        Contex::DefaultContext().UseTheme(style);
    }

    void SetFont(font::FontAtlas* font)
    {
        Contex::DefaultContext().GetTheme()->font = font;
        uint16_t tex_id = 0;
        std::tie(tex_id, std::ignore) = font->Tex2D();
        Contex::DefaultContext().GetDrawList().PushTextureId(tex_id);
    }

    // Set Z-Order for
    int16_t SetZOrder(int16_t z)
    {
        int16_t old = Contex::DefaultContext().GetDrawList().z_order_;
        Contex::DefaultContext().GetDrawList().z_order_ = z;
        return old;
    }

    // for internal usage, DO NOT call.
    void SetScreenSize(float w, float h)
    {
        SharedScreen.SetRealSize(w, h);
    }

    // ScreenSize return the physical width&height of the screen.
    std::tuple<float, float> ScreenSize()
    {
        return { SharedScreen.rl_width_, SharedScreen.rl_height_ };
    }

    std::tuple<float, float> VirtualSize()
    {
        return { SharedScreen.vt_width_, SharedScreen.vt_height_ };
    }

    std::tuple<float, float, float, float> HintAndScale()
    {
        return { SharedScreen.hint_x_, SharedScreen.hint_y_, SharedScreen.scale_x_, SharedScreen.scale_y_ };
    }

    // SetVirtualResolution set the virtual resolution.
    void SetVirtualResolution(float w, float h)
    {
        SharedScreen.SetVirtualSize(w, h);
    }

    Contex& DefaultContext()
    {
        return Contex::DefaultContext();
    }

} // namespace gui
} // namespace ant2d

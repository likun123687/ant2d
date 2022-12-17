#pragma once
#include <cstdint>
#include <gfx/font/font.h>
#include <math/vector.h>

namespace ant2d {
inline constexpr uint8_t Visible = 0;
inline constexpr uint8_t InVisible = Visible + 1;
inline constexpr uint8_t Gone = Visible + 2;

struct TextStyle {
    font::FontAtlas* font;
    uint32_t color;
    float size;
    int lines;
    float line_space;
};

struct ImageStyle {
    uint32_t tint;
};

struct CheckBoxStyle {
};

struct ProgressBarStyle {
};

struct SliderStyle {
    uint32_t bar;
    uint32_t knob;
};

struct Padding {
    float left;
    float right;
    float top;
    float bottom;
};

struct InputStyle {
    uint8_t visibility;
    uint32_t color;
    uint32_t hint_color;
    float size;
};

struct ButtonStyle {
    struct {
        uint32_t normal;
        uint32_t pressed;
    } back_ground;

    TextStyle text_style;
    Padding padding;
    math::Vec2 gravity;
    float rounding;
};

struct ImageButtonStyle {
    ImageStyle image_style;
    Padding padding;
};

struct Theme {
    TextStyle text;
    ButtonStyle button;
    ImageStyle image;
    ImageButtonStyle image_button;
    SliderStyle slider;

    // global config..
    uint32_t normal;
    uint32_t pressed;

    // item 之间的空隙
    float spacing;

    // default font
    font::FontAtlas* font;

    static Theme& LightTheme()
    {
        static Theme theme;
        static bool is_init = false;
        if (!is_init) {
            theme.text.color = 0xFFFFFFFF;
            theme.text.size = 18;
            theme.text.line_space = 6;

            theme.button.text_style.color = 0xFFFFFFFF;
            theme.button.text_style.size = 24;
            theme.button.gravity = math::Vec2 { 0.5, 0.5 };
            theme.button.rounding = 5;

            theme.image.tint = 0xFFFFFFFF;
            theme.image_button.image_style.tint = 0xFFFFFFFF;
            auto& padding = theme.image_button.padding;
            padding.left = 0;
            padding.right = 0;
            padding.bottom = 0;
            padding.top = 0;

            theme.slider.bar = 0xFFFFFFFF;
            theme.slider.knob = 0xFFFFFFFF;

            theme.normal = 0xFFFFFFFF;
            theme.pressed = 0xFFFFFFFF;
            theme.spacing = 4;
        }

        return theme;
    }

    static Theme& DarkTheme()
    {
        static Theme theme;
        return theme;
    }
};

} // namespace ant2d

#define SharedContent \
    ant2d::Singleton<ant2d::Content>()

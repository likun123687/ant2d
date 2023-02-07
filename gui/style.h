#pragma once
#include <cstdint>
#include <gfx/font/font.h>
#include <math/vector.h>
#include <gfx/color.h>

namespace ant2d {
namespace gui {
    inline constexpr uint8_t Visible = 0;
    inline constexpr uint8_t InVisible = Visible + 1;
    inline constexpr uint8_t Gone = Visible + 2;

    struct TextStyle {
        font::FontAtlas* font;
        color::Byte4 color;
        float size;
        int lines;
        float line_space;
    };

    struct ImageStyle {
        color::Byte4 tint;
    };

    struct CheckBoxStyle {
    };

    struct ProgressBarStyle {
    };

    struct SliderStyle {
        color::Byte4 bar;
        color::Byte4 knob;
    };

    struct Padding {
        float left;
        float right;
        float top;
        float bottom;
    };

    struct InputStyle {
        uint8_t visibility;
        color::Byte4 color;
        color::Byte4 hint_color;
        float size;
    };

    struct ButtonStyle {
        struct {
            color::Byte4 normal;
            color::Byte4 pressed;
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
        color::Byte4 normal;
        color::Byte4 pressed;

        // item 之间的空隙
        float spacing;

        // default font
        font::FontAtlas* font;

        static Theme& LightTheme()
        {
            static Theme theme;
            static bool is_init = false;
            if (!is_init) {
                theme.text.color = color::Byte4 { 0x00, 0x00, 0x00, 0xFF };
                theme.text.size = 12;
                theme.text.line_space = 6;

                theme.button.text_style.color = color::Byte4 { 0x00, 0x00, 0x00, 0xFF };
                theme.button.text_style.size = 12;
                theme.button.gravity = math::Vec2 { 0.5, 0.5 };
                theme.button.rounding = 5;
                theme.button.padding.left = 10;
                theme.button.padding.right = 10;
                theme.button.padding.bottom = 10;
                theme.button.padding.top = 10;

                theme.image.tint = color::Byte4 { 0xFF, 0xFF, 0xFF, 0xFF };
                theme.image_button.image_style.tint = color::Byte4 { 0xFF, 0xFF, 0xFF, 0xFF };
                auto& padding = theme.image_button.padding;
                padding.left = 0;
                padding.right = 0;
                padding.bottom = 0;
                padding.top = 0;

                theme.slider.bar = color::Byte4 { 0xCC, 0xCC, 0xCC, 0xFF };
                theme.slider.knob = color::Byte4 { 0x88, 0x88, 0x88, 0xFF };

                theme.normal = color::Byte4 { 0xCC, 0xCC, 0xCC, 0xFF };
                theme.pressed = color::Byte4 { 0x88, 0x88, 0x88, 0xFF };
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
}
} // namespace ant2d

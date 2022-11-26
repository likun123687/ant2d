#pragma once
#include <asset/image_data.h>
#include <gfx/dbg/font_image.h>

namespace ant2d {
struct DebugFontSystem {
    static constexpr float font_width = 12.0f;
    static constexpr float font_height = 32.0f;
    static constexpr float font_x = 12.0f / 999;
    static constexpr float font_y = 32.0f / 54;

    static std::tuple<float, float, float, float> GlyphRegion(uint8_t text)
    {
        uint8_t i = text - '.';
        float left = float(i) * font_x;
        float right = left + font_x;
        float bottom = 0;
        float top = font_y;
        return std::make_tuple(left, right, bottom, top);
    }

    static ImageData LoadFontImage()
    {
        // return ImageData { debug_font_image, 12 * 32 * 4 };
        return ImageData { debug_font_image, sizeof(debug_font_image) };
        //return ImageData { "assets/debug_font.png" };
    }
};
} // namespace ant2d

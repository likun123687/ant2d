#pragma once

#include <cstdint>
namespace ant2d {
namespace font {
    struct Point {
        float x;
        float y;
    };

    // A Glyph describes metrics for a single Font glyph.
    // These indicate which area of a given image contains the
    // glyph data and how the glyph should be spaced in a rendered string.
    //
    // Advance determines the distance to the next glyph.
    // This is used to properly align non-monospaced fonts.
    struct Glyph {
        uint32_t rune;

        float x;
        float y;
        float width;
        float height;

        float xoffset;
        float yoffset;

        int advance;
        bool IsEmpty() const
        {
            return (width == 0.0) || (height == 0.0);
        }
    };
} // namespace font
}
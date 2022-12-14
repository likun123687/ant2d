
#pragma once
#include <gfx/font/font.h>

namespace ant2d {
namespace font {
    namespace bitmap {
        // LoadBitmap loads a bitmap (raster) fontAtlas from the given
        // sprite sheet and config files. It is optionally scaled by
        // the given scale factor.
        //
        // A scale factor of 1 retains the original size. A factor of 2 doubles the
        // fontAtlas size, etc. A scale factor of 0 is not valid and will default to 1.
        //
        // Supported image formats are 32-bit RGBA as PNG, JPEG.
        std::unique_ptr<FontAtlas> LoadBitmap(const std::string& filename, const std::string& config, int scale = 1);
    } // namespace ant2d
}
}
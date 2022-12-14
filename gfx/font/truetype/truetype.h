#pragma once
#include <gfx/font/font.h>
#include <gfx/font/truetype/config.h>
#include <unordered_map>

namespace ant2d {
namespace font {
    namespace truetype {
        // http://www.freetype.org/freetype2/docs/tutorial/step2.html

        // LoadTrueType loads a truetype fontAtlas from the given stream and
        // applies the given fontAtlas scale in points.
        //
        // The low and high values determine the lower and upper rune limits
        // we should load for this Font. For standard ASCII this would be:32, 127.
        using Glyphs = std::unordered_map<std::uint32_t, Glyph>;

        std::unique_ptr<FontAtlas> LoadTrueType(const Config& config);

    } // namespace ant2d
}
}

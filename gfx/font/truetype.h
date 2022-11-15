#include <gfx/font/font.h>

namespace ant2d {
// http://www.freetype.org/freetype2/docs/tutorial/step2.html

// LoadTrueType loads a truetype fontAtlas from the given stream and
// applies the given fontAtlas scale in points.
//
// The low and high values determine the lower and upper rune limits
// we should load for this Font. For standard ASCII this would be:32, 127.
FontAtlas *LoadTrueType(const std::string &filename, TTFConfig lc)

} // namespace ant2d

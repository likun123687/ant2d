#pragma once

#include <set>
#include <cstdint>
#include <string>
#include <vector>

namespace ant2d {
namespace font {
namespace truetype {

    struct Config {
        enum class DataFormat {
            Xml,
            Text,
            Bin,
            Json,
            Cbor
        };

        enum class KerningPairs {
            Disabled,
            Basic,
            Regular,
            Extended
        };

        enum class TextureNameSuffix {
            IndexAligned,
            Index,
            None
        };

        struct Color {
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;

            std::uint32_t getBGR() const
            {
                return r + (g << 8u) + (b << 16u);
            }

            bool operator==(const Color& other) const
            {
                return r == other.r && g == other.b && b == other.b;
            }
        };

        struct Size {
            Size() = default;
            Size(std::uint32_t w, std::uint32_t h)
                : w(w)
                , h(h)
            {
            }
            std::uint32_t w = 0;
            std::uint32_t h = 0;
        };

        struct Padding {
            std::uint32_t up = 0;
            std::uint32_t right = 0;
            std::uint32_t down = 0;
            std::uint32_t left = 0;
        };

        struct Spacing {
            std::uint32_t ver = 0;
            std::uint32_t hor = 0;
        };

        struct Alignment {
            std::uint32_t ver = 1;
            std::uint32_t hor = 1;
        };

        Config(uint32_t low, uint32_t high, uint16_t font_size = 16)
            : fontSize { font_size }
        {
            if (low == high && low == 0) {
                low = 32;
                high = 127;
            }

            for (auto i = low, j = high; i <= high; i++) {
                chars.insert(i);
            }
        }

        std::string fontFile;
        std::set<std::uint32_t> chars;
        Color color;
        Color backgroundColor;
        bool backgroundTransparent = true;
        std::uint16_t fontSize = 16;
        Padding padding;
        Spacing spacing;
        Alignment alignment;
        std::vector<Size> textureSizeList;
        std::string output;
        DataFormat dataFormat = DataFormat::Text;
        KerningPairs kerningPairs = KerningPairs::Disabled;
        std::uint32_t maxTextureCount = 1;
        bool useMaxTextuerCount = false;
        bool monochrome = false;
        bool extraInfo = false;
        bool cropTexturesWidth = false;
        bool cropTexturesHeight = false;
        bool verbose = false;
        TextureNameSuffix textureNameSuffix = TextureNameSuffix::IndexAligned;
    };

} // namespace truetype

}
}
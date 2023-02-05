#pragma once
#include <unordered_map>
#include <string>
#include <gfx/font/font.h>
#include <gfx/font/bitmap/bitmap.h>
#include <gfx/font/truetype/truetype.h>

namespace ant2d {

class FontManager {
public:
    struct FontCount {
        std::unique_ptr<font::FontAtlas> ref;
        uint32_t cnt;
    };
    void Loadbitmap(const std::string& name, const std::string& file, const std::string& fc);
    void LoadTrueType(const std::string& name, const std::string& file, font::truetype::Config config);
    void Unload(const std::string& name);

    font::FontAtlas* Get(const std::string& name);

private:
    std::unordered_map<std::string, FontCount> repo_;
};
} // namespace ant2d

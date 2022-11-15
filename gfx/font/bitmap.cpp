#include <gfx/font/bitmap.h>
#include <utils/json.h>

namespace ant2d {
std::unique_ptr<IFont> LoadBitmap(const std::string& filename, const std::string& config, int scale)
{
    FontAtlas* f = new FontAtlas {};
    if (scale == 1) {
        auto image_data = ImageData { filename };
        image_data.SetColor(0, 0, 0xff, 0xff, 0xff, 0xff);
        f->LoadTex(image_data);
    } else {
        auto image_data = ImageData { filename };
        auto scale_image_data = ImageData::Scale(image_data);
        f->LoadTex(scale_image_data);
    }

    auto [config_file_data, config_file_len] = SharedContent.LoadFile(config);
    nlohmann::json config_json = nlohmann::json::parse(config_file_data.get(), config_file_data.get() + config_file_len);

    if (config_json.is_discarded()) {
        Error("parse atlas file {} error", config);
    }

    int gh = 0;
    int gw = 0;
    // add glyphs
    for (auto& g : desc_json["glyphs"].items()) {
        auto glyph = Glyph {};
        glyph.rune = g["id"];
        glyph.x = g["x"];
        glyph.y = g["y"];
        glyph.width = g["width"];
        glyph.height = g["height"];
        glyph.xoffset = g["xoffset"];
        glyph.yoffset = g["yoffset"];
        glyph.advance = g["advance"];
        f->AddGlyphs(glyph);

        if (g["width"] > gw) {
            gw = g["width"];
        }

        if (g["height"] > gh) {
            gh = g["height"];
        }
    }

    f->SetGWidth(gw);
    f->SetGWidth(gh);
    return f;
}
} //namespace ant2d
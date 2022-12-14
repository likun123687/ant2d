#include <gfx/font/bitmap/bitmap.h>
#include <utils/content.h>
#include <utils/json.h>

namespace ant2d {
namespace font {
    namespace bitmap {
        std::unique_ptr<FontAtlas> LoadBitmap(const std::string& filename, const std::string& config, int scale)
        {
            std::unique_ptr<FontAtlas> f { new FontAtlas };
            if (scale == 1) {
                auto image_data = ImageData { filename };
                image_data.SetColor(0, 0, 0xff, 0xff, 0xff, 0xff);
                f->LoadTex(image_data);
            } else {
                auto image_data = ImageData { filename };
                auto scale_image_data = ImageData::Scale(image_data, scale);
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
            for (auto& g : config_json["glyphs"].items()) {
                auto glyph = Glyph {};
                const auto& value = g.value();
                glyph.rune = value["id"];
                glyph.x = value["x"];
                glyph.y = value["y"];
                glyph.width = value["width"];
                glyph.height = value["height"];
                glyph.xoffset = value["xoffset"];
                glyph.yoffset = value["yoffset"];
                glyph.advance = value["advance"];
                f->AddGlyphs(value["id"], glyph);

                if (value["width"] > gw) {
                    gw = value["width"];
                }

                if (value["height"] > gh) {
                    gh = value["height"];
                }
            }

            f->SetGWidth(gw);
            f->SetGWidth(gh);
            return std::move(f);
        }
    }
} // namespace font
}
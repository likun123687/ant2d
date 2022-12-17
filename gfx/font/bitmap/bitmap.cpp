#include <gfx/font/bitmap/bitmap.h>
#include <utils/content.h>
#include <utils/json.h>

namespace ant2d {
namespace font {
    namespace bitmap {
        inline int json_string_to_int(nlohmann::json& value)
        {
            return std::stoi(value.get_ref<std::string&>());
        }

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
                auto& value = g.value();
                glyph.rune = json_string_to_int(value["id"]);
                glyph.x = json_string_to_int(value["x"]);
                glyph.y = json_string_to_int(value["y"]);
                glyph.width = json_string_to_int(value["width"]);
                glyph.height = json_string_to_int(value["height"]);
                glyph.xoffset = json_string_to_int(value["xoffset"]);
                glyph.yoffset = json_string_to_int(value["yoffset"]);
                glyph.advance = json_string_to_int(value["advance"]);
                f->AddGlyphs(json_string_to_int(value["id"]), glyph);

                if (glyph.width > gw) {
                    gw = glyph.width;
                }

                if (glyph.height > gh) {
                    gh = glyph.height;
                }
            }

            f->SetGWidth(gw);
            f->SetGHeight(gh);
            return std::move(f);
        }
    }
} // namespace font
}
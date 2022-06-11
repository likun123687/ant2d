#pragma once
#include <asset/res.h>
#include <utils/json.h>

namespace ant2d {
class TextureManager {
    using json = nlohmann::json;

public:
    void Load(const std::string& file);
    void Unload(const std::string& file);
    void LoadAtlas(const std::string& file, const std::string& desc);
    void LoadAtlasIndexed(const std::string& file, float width, float height, int row, int col);

private:
    std::unordered_map<std::string, IdCount> repo_;
    std::unordered_map<std::string, uint32_t> names_;

    uint16_t CreateTextureByImageFile(const std::string& image);
    std::tuple<uint16_t, json> CreateTextureByAtlasFile(const std::string& image, const std::string& desc);
};
}

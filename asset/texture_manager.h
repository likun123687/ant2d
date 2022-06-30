#pragma once
#include <asset/res.h>
#include <utils/json.h>
#include <gfx/bk_texture.h>
//#include <utils/singleton.h>

namespace ant2d {
class TextureManager {
    using json = nlohmann::json;

public:
    void Load(const std::string& file);
    void Unload(const std::string& file);
    void LoadAtlas(const std::string& file, const std::string& desc);
    void LoadAtlasIndexed(const std::string& file, float width, float height, int row, int col);
    // Get returns the low-level Texture.
    BkTexture* Get(const std::string& file);
    std::tuple<uint16_t, Texture2D*> GetRaw(const std::string &file);
    Atlas *GetAtlas(const std::string &file);

#ifdef ANT2D_DEBUG
    auto &GetRepo()
    {
        return repo_;
    }

#endif

private:
    std::unordered_map<std::string, IdCount> repo_;
    std::unordered_map<std::string, std::unique_ptr<BkTexture>> textures_;

    uint16_t CreateTextureByImageFile(const std::string& image);
    std::tuple<uint16_t, json> CreateTextureByAtlasFile(const std::string& image, const std::string& desc);
};
}


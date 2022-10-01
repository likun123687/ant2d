#include <asset/texture_manager.h>
#include <gfx/bk/res_manager.h>
#include <utils/content.h>
#include <utils/debug.h>

namespace ant2d {
void TextureManager::Load(const std::string& file)
{
    uint16_t rid = 0;
    uint16_t cnt = 0;

    auto it = repo_.find(file);
    if (it != repo_.end()) {
        cnt = it->second.cnt;
        rid = it->second.rid;
    } else {
        rid = CreateTextureByImageFile(file);
    }
    repo_[file] = IdCount { rid, static_cast<uint16_t>(cnt + 1) };
}

void TextureManager::Unload(const std::string& file)
{
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        if (it->second.cnt > 1) {
            it->second.cnt -= 1;
        } else {
            repo_.erase(file);
            textures_.erase(file);
            SharedResManager.Free(it->second.rid);
            SharedAtlasManager.DeleteAtlasByName(file);
        }
    }
}

void TextureManager::LoadAtlas(const std::string& file, const std::string& desc)
{
    uint16_t rid = 0;
    uint16_t cnt = 0;

    auto it = repo_.find(file);
    if (it != repo_.end()) {
        cnt = it->second.cnt;
        rid = it->second.rid;
    } else {
        auto [tex_id, desc_json] = CreateTextureByAtlasFile(file, desc);
        auto size = desc_json["frames"].size();
        auto atlas = SharedAtlasManager.NewAtlas(tex_id, size, file);
        for (auto& [key, f] : desc_json["frames"].items()) {
            atlas->AddItem(f["frame"]["x"], f["frame"]["y"], f["frame"]["w"], f["frame"]["h"], key, f["rotated"]);
        }

        rid = tex_id;
    }
    repo_[file] = IdCount { rid, static_cast<uint16_t>(cnt + 1) };
}

void TextureManager::LoadAtlasIndexed(const std::string& file, float width, float height, int row, int col)
{
    uint16_t rid = 0;
    uint16_t cnt = 0;

    auto it = repo_.find(file);
    if (it != repo_.end()) {
        cnt = it->second.cnt;
        rid = it->second.rid;
    } else {
        auto tex_id = CreateTextureByImageFile(file);
        auto size = row * col;
        auto atlas = SharedAtlasManager.NewAtlas(tex_id, size, file);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                atlas->AddItem(j * width, i * height, width, height, "", false);
            }
        }

        rid = tex_id;
    }
    repo_[file] = IdCount { rid, static_cast<uint16_t>(cnt + 1) };
}

BkTexture* TextureManager::Get(const std::string& file)
{
    auto tex_it = textures_.find(file);
    if (tex_it != textures_.end()) {
        return tex_it->second.get();
    }

    auto it = repo_.find(file);
    if (it != repo_.end()) {
        auto rid = it->second.rid;
        auto tex = new BkTexture { rid };
        textures_[file] = std::unique_ptr<BkTexture>(tex);
        return tex;
    }

    return nullptr;
}

std::tuple<uint16_t, Texture2D*> TextureManager::GetRaw(const std::string& file)
{
    uint16_t tex_id = 0;
    Texture2D* tex = nullptr;
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        tex_id = it->second.rid;
        tex = SharedResManager.GetTexture(tex_id);
    }
    return std::make_tuple(tex_id, tex);
}

Atlas* TextureManager::GetAtlas(const std::string& file)
{
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        return SharedAtlasManager.GetAtlasByName(file);
    }
    return nullptr;
}

uint16_t TextureManager::CreateTextureByImageFile(const std::string& image)
{
    auto data = SharedContent.LoadFile(image);
    auto image_data = ImageData(data.first.get(), data.second);
    uint16_t texture_id = 0;
    std::tie(texture_id, std::ignore) = SharedResManager.AllocTexture(image_data);
    return texture_id;
}

std::tuple<uint16_t, TextureManager::json> TextureManager::CreateTextureByAtlasFile(const std::string& image, const std::string& desc)
{
    uint16_t tex_id = CreateTextureByImageFile(image);
    auto [desc_file_data, desc_file_len] = SharedContent.LoadFile(desc);
    json desc_json = json::parse(desc_file_data.get(), desc_file_data.get() + desc_file_len);

    if (desc_json.is_discarded()) {
        Error("parse atlas file {} error", desc);
    }

    return std::make_tuple(tex_id, desc_json);
}
}

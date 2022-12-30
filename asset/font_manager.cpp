#include <asset/font_manager.h>
#include <utils/content.h>

namespace ant2d {
void FontManager::Loadbitmap(const std::string& name, const std::string& file, const std::string& fc)
{
    int32_t cnt = 0;

    auto it = repo_.find(name);
    if (it != repo_.end()) {
        it->second.cnt += 1;
    } else {
        std::unique_ptr<font::FontAtlas> fnt = font::bitmap::LoadBitmap(file, fc, 1);
        repo_[name] = FontCount { std::move(fnt), 1 };
    }
}

void FontManager::LoadTrueType(const std::string& name, const std::string& file, font::truetype::Config config)
{
    int32_t cnt = 0;
    if (config.fontFile.empty()) {
        config.fontFile = SharedContent.GetFullPath(file);
    }

    auto it = repo_.find(name);
    if (it != repo_.end()) {
        it->second.cnt += 1;
    } else {
        std::unique_ptr<font::FontAtlas> fnt = font::truetype::LoadTrueType(config);
        repo_[name] = FontCount { std::move(fnt), 1 };
    }
}

void FontManager::Unload(const std::string& name)
{
    auto it = repo_.find(name);
    if (it != repo_.end()) {
        if (it->second.cnt > 1) {
            it->second.cnt -= 1;
        } else {
            it->second.ref->Dispose();
            repo_.erase(name);
        }
    }
}

font::FontAtlas* FontManager::Get(const std::string& name)
{
    auto it = repo_.find(name);
    if (it != repo_.end()) {
        return it->second.ref.get();
    }
    return nullptr;
}
} // namespace ant2d

#pragma once

#include <cinttypes>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <utils/Own.h>
#include <utils/Singleton.h>
#include <vector>

namespace ant2d {
class Content {
public:
    std::string GetFullPath(const std::string& file_name);
    std::pair<OwnArray<uint8_t>, size_t> LoadFile(const std::string& file_name);
    bool IsAbsolutePath(const std::string& str_path);
    bool IsFileExist(const std::string& file_path);
    std::string GetFullPathForDirectoryAndFilename(const std::string& directory, const std::string& file_name);
    std::string GetAssetPath();
    virtual ~Content();
    Content();

private:
    std::uint8_t* LoadFileUnsafe(const std::string& file_name, int64_t& size);
    std::string asset_path_;
    std::vector<std::string> search_paths_;
    std::unordered_map<std::string, std::string> full_path_cache_;
};

#define SharedContent \
    ant2d::Singleton<ant2d::Content>()
} // namespace ant2d

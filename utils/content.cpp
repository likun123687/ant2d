#include <cfile.hpp>
#include <filesystem>
#include <utils/Content.h>
#include <utils/Utils.h>
#include <utils/silly/Slice.h>

using namespace silly::slice;
namespace fs = std::filesystem;
using namespace ant2d;

#if BK_PLATFORM_WINDOWS
Content::Content()
{
    asset_path_ = fs::current_path().string();
}
#endif

Content::~Content()
{
}

#if BK_PLATFORM_WINDOWS
bool Content::IsFileExist(const std::string& file_path)
{
    auto str_path = fs::path(file_path);
    if (!IsAbsolutePath(file_path)) {
        str_path = fs::path(asset_path_) / str_path;
    }
    return fs::exists(str_path);
}
#endif

#if BK_PLATFORM_WINDOWS
bool Content::IsAbsolutePath(const std::string& str_path)
{
    return fs::path(str_path).is_absolute();
    // if (strPath.size() > 2
    //     && ((strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z'))
    //     && strPath[1] == ':')
    //{
    //     return true;
    // }
    // return false;
}
#endif

#if BK_PLATFORM_WINDOWS
std::string Content::GetFullPathForDirectoryAndFilename(const std::string& directory, const std::string& file_name)
{
    auto rootPath = fs::path(IsAbsolutePath(directory) ? "" : asset_path_);
    std::string fullPath = (rootPath / directory / file_name).string();
    if (!IsFileExist(fullPath)) {
        fullPath.clear();
    }
    return fullPath;
}
#endif

std::string Content::GetAssetPath()
{
    return asset_path_;
}

static std::tuple<std::string, std::string> SplitDirectoryAndFilename(const std::string& filePath)
{
    std::string file = filePath;
    std::string path;
    std::size_t pos = filePath.find_last_of("/\\");
    if (pos != std::string::npos) {
        path = filePath.substr(0, pos + 1);
        file = filePath.substr(pos + 1);
    }
    return std::make_tuple(path, file);
}

std::string Content::GetFullPath(const std::string& file_name)
{
    Slice targetFile = file_name;
    targetFile.trimSpace();

    while (targetFile.size() > 1 && (targetFile.back() == '\\' || targetFile.back() == '/')) {
        targetFile.skipRight(1);
    }

    if (IsAbsolutePath(targetFile)) {
        return targetFile;
    }

    auto it = full_path_cache_.find(targetFile);
    if (it != full_path_cache_.end()) {
        return it->second;
    }

    std::string path, file, fullPath;
    auto fname = fs::path(targetFile.begin(), targetFile.end()).lexically_normal();
    for (const auto& searchPath : search_paths_) {
        std::tie(path, file) = SplitDirectoryAndFilename((fs::path(searchPath) / fname).string());
        fullPath = GetFullPathForDirectoryAndFilename(path, file);
        if (!fullPath.empty()) {
            full_path_cache_[fname.string()] = fullPath;
            return fullPath;
        }
    }

    std::tie(path, file) = SplitDirectoryAndFilename(targetFile);
    fullPath = GetFullPathForDirectoryAndFilename(path, file);
    if (!fullPath.empty()) {
        full_path_cache_[targetFile] = fullPath;
        return fullPath;
    }

    return fullPath;
}

std::pair<OwnArray<uint8_t>, size_t> Content::LoadFile(const std::string& file_name)
{
    int64_t size = 0;
    uint8_t* data = Content::LoadFileUnsafe(file_name, size);
    return { OwnArray<uint8_t>(data), s_cast<size_t>(size) };
}

uint8_t* Content::LoadFileUnsafe(const std::string& file_name, int64_t& size)
{
    if (file_name.empty()) {
        return nullptr;
    }
    std::string fullPath = Content::GetFullPath(file_name);
    auto file_io = xtr::cfile(fullPath.c_str(), xtr::cfile::mode::read | xtr::cfile::mode::binary);

    size = fs::file_size(fullPath);
    auto buffer = new uint8_t[s_cast<size_t>(size)];
    file_io.fread<uint8_t>(buffer, s_cast<size_t>(size));
    return buffer;
}

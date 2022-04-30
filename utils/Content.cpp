#include <utils/Content.h>
#include <filesystem>
#include<utils/Utils.h>
#include <cfile.hpp>
#include <utils/silly/Slice.h>

using namespace silly::slice;
namespace fs = std::filesystem;
using namespace ant2d;

#if BK_PLATFORM_WINDOWS
Content::Content()
{
    _assetPath = fs::current_path().string();
}
#endif

Content::~Content()
{ }

#if BK_PLATFORM_WINDOWS
bool Content::isFileExist(const std::string& filePath)
{
    auto strPath = fs::path(filePath);
    if (!isAbsolutePath(filePath))
    {
        strPath = fs::path(_assetPath) / strPath;
    }
    return fs::exists(strPath);
}
#endif


#if BK_PLATFORM_WINDOWS
bool Content::isAbsolutePath(const std::string& strPath)
{
    return fs::path(strPath).is_absolute();
    //if (strPath.size() > 2
    //    && ((strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z'))
    //    && strPath[1] == ':')
    //{
    //    return true;
    //}
    //return false;
}
#endif

#if BK_PLATFORM_WINDOWS
std::string Content::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename)
{
    auto rootPath = fs::path(isAbsolutePath(directory) ? "" : _assetPath);
    std::string fullPath = (rootPath / directory / filename).string();
    if (!isFileExist(fullPath))
    {
        fullPath.clear();
    }
    return fullPath;
}
#endif

std::string Content::getAssetPath()
{
    return _assetPath;
}

static std::tuple<std::string, std::string> splitDirectoryAndFilename(const std::string& filePath)
{
    std::string file = filePath;
    std::string path;
    std::size_t pos = filePath.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        path = filePath.substr(0, pos + 1);
        file = filePath.substr(pos + 1);
    }
    return std::make_tuple(path, file);
}

std::string Content::getFullPath(const std::string& filename)
{
    Slice targetFile = filename;
    targetFile.trimSpace();

    while (targetFile.size() > 1 &&
        (targetFile.back() == '\\' || targetFile.back() == '/'))
    {
        targetFile.skipRight(1);
    }

    if (isAbsolutePath(targetFile))
    {
        return targetFile;
    }

    auto it  = _fullPathCache.find(targetFile);
    if (it != _fullPathCache.end())
    {
        return it->second;
    }

    std::string path, file, fullPath;
    auto fname = fs::path(targetFile.begin(), targetFile.end()).lexically_normal();
    for (const auto& searchPath : _searchPaths)
    {
        std::tie(path, file) = splitDirectoryAndFilename((fs::path(searchPath) / fname).string());
        fullPath = getFullPathForDirectoryAndFilename(path, file);
        if (!fullPath.empty())
        {
            _fullPathCache[fname.string()] = fullPath;
            return fullPath;
        }
    }

    std::tie(path, file) = splitDirectoryAndFilename(targetFile);
    fullPath = getFullPathForDirectoryAndFilename(path, file);
    if (!fullPath.empty())
    {
        _fullPathCache[targetFile] = fullPath;
        return fullPath;
    }

    return fullPath;
}

std::pair<OwnArray<uint8_t>, size_t> Content::loadFile(const std::string& filename)
{
    int64_t size = 0;
    uint8_t* data = Content::_loadFileUnsafe(filename, size);
    return {OwnArray<uint8_t>(data), s_cast<size_t>(size)};
}

uint8_t* Content::_loadFileUnsafe(const std::string& filename, int64_t& size)
{
    if (filename.empty()) {
        return nullptr;
    }
    std::string fullPath = Content::getFullPath(filename);
    auto file_io = xtr::cfile(fullPath.c_str(), xtr::cfile::mode::read | xtr::cfile::mode::binary);

    size = fs::file_size(fullPath);
    auto buffer = new uint8_t[s_cast<size_t>(size)];
    file_io.fread<uint8_t>(buffer, s_cast<size_t>(size));
    return buffer;
}

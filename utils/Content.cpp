#include <utils/Content.h>
#include <filesystem>
#include<utils/Utils.h>
#include <cfile.hpp>

#if BK_PLATFORM_WINDOWS
#include <windows.h>
#endif // BK_PLATFORM_WINDOWS

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
bool Content::isFileExist(const Slice& filePath)
{
    std::string strPath = filePath;
    if (!isAbsolutePath(strPath))
    {
        strPath.insert(0, _assetPath);
    }
    return GetFileAttributesA(strPath.c_str()) != -1 ? true : false;
}
#endif


#if BK_PLATFORM_WINDOWS
bool Content::isAbsolutePath(const Slice& strPath)
{
    if (strPath.size() > 2
        && ((strPath[0] >= 'a' && strPath[0] <= 'z') || (strPath[0] >= 'A' && strPath[0] <= 'Z'))
        && strPath[1] == ':')
    {
        return true;
    }
    return false;
}
#endif

#if BK_PLATFORM_WINDOWS
std::string Content::getFullPathForDirectoryAndFilename(const Slice& directory, const Slice& filename)
{
    auto rootPath = fs::path(isAbsolutePath(directory) ? Slice::Empty : _assetPath);
    std::string fullPath = (rootPath / directory.toString() / filename.toString()).string();
    if (!isFileExist(fullPath))
    {
        fullPath.clear();
    }
    return fullPath;
}
#endif

std::string Content::getAssertPath()
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

std::string Content::getFullPath(const Slice& filename)
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

    return targetFile;
}

std::pair<OwnArray<std::uint8_t>,std::size_t> Content::loadFile(const Slice& filename)
{
    std::int64_t size = 0;
    std::uint8_t* data = Content::_loadFileUnsafe(filename, size);
    return {OwnArray<std::uint8_t>(data), s_cast<std::size_t>(size)};
}

std::uint8_t* Content::_loadFileUnsafe(const Slice& filename, std::int64_t& size)
{
    if (filename.empty()) {
        return nullptr;
    }
    std::string fullPath = Content::getFullPath(filename);
    auto file_io = xtr::cfile(fullPath.c_str(), xtr::cfile::mode::read | xtr::cfile::mode::binary);

    size = fs::file_size(fullPath);
    auto buffer = new std::uint8_t[s_cast<std::size_t>(size)];
    file_io.fread<std::uint8_t>(buffer, s_cast<std::size_t>(size));
    return buffer;
}

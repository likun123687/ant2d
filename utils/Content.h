#pragma once

#include <string>
#include <cinttypes>
#include <cstddef>

#include <utils/Own.h>
#include <utils/silly/Slice.h>
#include <unordered_map>
#include <vector>
#include <utils/Singleton.h>

using namespace silly::slice;

namespace ant2d {
class Content
{
public:
    std::string getFullPath(const Slice& filename);
    std::pair<OwnArray<std::uint8_t>,std::size_t> loadFile(const Slice& filename);
    bool isAbsolutePath(const Slice& strPath);
    bool isFileExist(const Slice& filePath); //只能检测绝对路径
    std::string getFullPathForDirectoryAndFilename(const Slice& directory, const Slice& filename);
    std::string getAssertPath();
    virtual ~Content();
protected:
    Content();
private:
    std::uint8_t* _loadFileUnsafe(const Slice& filename, std::int64_t& size);
    std::string _assetPath;
    std::vector<std::string> _searchPaths;
    std::unordered_map<std::string, std::string> _fullPathCache;
};

#define SharedContent \
    ant2d::Singleton<ant2d::Content>::shared()
} //namespace ant2d

#pragma once

#include <string>
#include <cinttypes>
#include <cstddef>
#include <utils/Own.h>
#include <unordered_map>
#include <vector>
#include <utils/Singleton.h>

namespace ant2d {
class Content
{
public:
    std::string getFullPath(const std::string& filename);
    std::pair<OwnArray<uint8_t>, size_t> loadFile(const std::string& filename);
    bool isAbsolutePath(const std::string& strPath);
    bool isFileExist(const std::string& filePath); 
    std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename);
    std::string getAssetPath();
    virtual ~Content();
protected:
    Content();
private:
    std::uint8_t* _loadFileUnsafe(const std::string& filename, int64_t& size);
    std::string _assetPath;
    std::vector<std::string> _searchPaths;
    std::unordered_map<std::string, std::string> _fullPathCache;
};

#define SharedContent \
    ant2d::Singleton<ant2d::Content>::shared()
} //namespace ant2d

#pragma once
#include <gfx/bk/res_manager.h>
#include <unordered_map>
#include <utils/singleton.h>

namespace ant2d {
struct Region {
    float x1;
    float y1;
    float x2;
    float y2;
    bool rotated;
};

struct Size {
    float width;
    float height;
};

class ITexture2D {
public:
    virtual uint16_t GetTextureId() = 0;
    virtual Region GetRegion() = 0;
    virtual Size GetSize() = 0;
    virtual ~ITexture2D() = default;
};

enum class AnchorType {
    kCenter,
    kLeft,
    kRight,
    kUp,
    kDown,
};

class BkTexture : public ITexture2D {
private:
    uint16_t texture_id_;
    uint16_t padding_;

public:
    BkTexture(uint16_t id);
    uint16_t GetTextureId();
    Region GetRegion();
    Size GetSize();
};

class SubTexture : public ITexture2D {
private:
    uint32_t id_;

public:
    SubTexture(uint32_t id);
    uint16_t GetTextureId();
    Region GetRegion();
    Size GetSize();
    std::tuple<uint16_t, uint16_t> SplitSubId();
};

class Atlas {
private:
    uint16_t texture_id_;
    uint16_t aid_;
    float width_;
    float height_;
    std::vector<Region> regions_;
    std::vector<Size> sizes_;
    std::unordered_map<std::string, int> names_;
    std::vector<std::unique_ptr<SubTexture>> sub_textures_;

    uint16_t index_;
    uint16_t size_;

public:
    Atlas(uint16_t aid);
    void Init(int size);
    void Release();
    void AddItem(float x, float y, float w, float h, const std::string& name, bool rotated);
    SubTexture* GetSubTexByName(const std::string& name);
    SubTexture* GetSubTexByIndex(int index);
    Region GetSubTexRegionByIndex(int index);
    Size GetSubTexSizeByIndex(int index);
    void SetTexId(uint16_t id);
    uint16_t GetTextureId();
    void SetWidth(float width);
    void SetHeight(float height);
};

class AtlasManager {
private:
    std::vector<std::unique_ptr<Atlas>> atlases_;
    std::vector<uint16_t> frees_;
    std::unordered_map<std::string, int> names_;
    uint16_t index_;

public:
    Atlas* NewAtlas(uint16_t id, int size, const std::string& name);
    void DeleteAtlasByName(const std::string& name);
    Atlas* GetAtlasByName(const std::string& name);
    Region GetSubRegionBySubId(uint32_t id);
    Size GetSubSizeBySubId(uint32_t id);
    uint16_t GetTexIdBySubId(uint32_t id);
};

} // namespace ant2d

#define SharedAtlasManager \
    ::ant2d::Singleton<::ant2d::AtlasManager>()

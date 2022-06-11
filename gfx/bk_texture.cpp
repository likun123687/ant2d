#include <gfx/bk_texture.h>

namespace ant2d {

BkTexture::BkTexture(uint16_t id)
    : texture_id_(id)
{
}

uint16_t BkTexture::GetTextureId()
{
    return static_cast<uint16_t>(texture_id_);
}

Region BkTexture::GetRegion()
{
    return Region { 0, 0, 1, 1, false };
}

Size BkTexture::GetSize()
{
    Size size = Size();
    Texture2D* t = SharedResManager.GetTexture(texture_id_);
    if (t) {
        size.width = t->GetWidth();
        size.height = t->GetHeight();
    }
    return size;
}

Atlas::Atlas(uint16_t aid)
{
    aid_ = aid;
}

void Atlas::Init(int size)
{
    index_ = 0;
    size_ = size;
    regions_.reserve(size);
    sizes_.reserve(size);
    names_.reserve(size);
}

void Atlas::Release()
{
    regions_.clear();
    sizes_.clear();
    names_.clear();
}

void Atlas::AddItem(float x, float y, float w, float h, const std::string& name, bool rotated)
{
    sizes_[index_] = Size { w, h };
    names_[name] = index_;
    if (rotated) {
        regions_[index_] = Region { x / width_, y / height_,
            (x + height_) / width_, (y + width_) / height_, true };
    } else {
        regions_[index_] = Region { x / width_, y / height_,
            (x + height_) / width_, (y + width_) / height_, false };
    }
    index_++;
}

std::tuple<SubTexture, bool> Atlas::GetSubTexByName(const std::string& name)
{
    bool ok = false;
    auto it = names_.find(name);
    if (it != names_.end()) {
        ok = true;
        auto sub_tex = SubTexture { (uint32_t(aid_) << 16) + uint32_t(it->second) };
        return std::make_tuple(sub_tex, ok);
    }
    return std::make_tuple(SubTexture { 0 }, ok);
}

std::tuple<SubTexture, bool> Atlas::GetSubTexByIndex(int index)
{
    bool ok = false;
    if (index < size_) {
        ok = true;
        auto sub_tex = SubTexture { (uint32_t(aid_) << 16) + uint32_t(index) };
        return std::make_tuple(sub_tex, ok);
    }

    return std::make_tuple(SubTexture { 0 }, ok);
}

Region Atlas::GetSubTexRegionByIndex(int index)
{
    return regions_[index];
}

Size Atlas::GetSubTexSizeByIndex(int index)
{
    return sizes_[index];
}

void Atlas::SetTexId(uint16_t id)
{
    texture_id_ = id;
}

uint16_t Atlas::GetTextureId()
{
    return texture_id_;
}

void Atlas::SetWidth(float width)
{
    width_ = width;
}

void Atlas::SetHeight(float height)
{
    height_ = height;
}

Atlas* AtlasManager::NewAtlas(uint16_t id, int size, const std::string& name)
{
    Atlas* atlas = nullptr;
    if (!frees_.empty()) {
        atlas = atlases_[frees_.back()].get();
        frees_.pop_back();
    } else {
        uint16_t ii = static_cast<uint16_t>(atlases_.size());
        atlases_.emplace_back(new Atlas { ii });
        atlas = atlases_[ii].get();
        names_[name] = ii;
    }

    atlas->Init(size);
    atlas->SetTexId(id);
    auto texture = SharedResManager.GetTexture(id);
    if (texture) {
        atlas->SetWidth(texture->GetWidth());
        atlas->SetHeight(texture->GetHeight());
    }
    return atlas;
}

void AtlasManager::DeleteAtlasByName(const std::string& name)
{
    auto it = names_.find(name);
    if (it != names_.end()) {
        auto index = it->second;
        atlases_[index]->Release();
        frees_.push_back(index);
        names_.erase(it);
    }
}

Atlas* AtlasManager::GetAtlasByName(const std::string& name)
{
    auto it = names_.find(name);
    if (it != names_.end()) {
        return atlases_[it->second].get();
    }
    return nullptr;
}

// Region returns sub-texture's Region by id.
Region AtlasManager::GetSubRegionBySubId(uint32_t id)
{
    auto ai = id >> 16;
    auto ii = id & 0xFFFF;

    auto& atlas = atlases_[ai];
    return atlas->GetSubTexRegionByIndex(ii);
}

Size AtlasManager::GetSubSizeBySubId(uint32_t id)
{
    auto ai = id >> 16;
    auto ii = id & 0xFFFF;

    auto& atlas = atlases_[ai];
    return atlas->GetSubTexSizeByIndex(ii);
}

uint16_t AtlasManager::GetTexIdBySubId(uint32_t id)
{
    return atlases_[id >> 16]->GetTextureId();
}

SubTexture::SubTexture(uint32_t id)
{
    id_ = id;
}

uint16_t SubTexture::GetTextureId()
{
    return SharedAtlasManager.GetTexIdBySubId(id_);
}

Region SubTexture::GetRegion()
{
    return SharedAtlasManager.GetSubRegionBySubId(id_);
}

Size SubTexture::GetSize()
{
    return SharedAtlasManager.GetSubSizeBySubId(id_);
}

std::tuple<uint16_t, uint16_t> SubTexture::SplitSubId()
{
    auto atlas = int(id_ >> 16);
    auto index = int(id_ & 0xFFFF);
    return std::make_tuple(atlas, index);
}
}

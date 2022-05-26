#pragma once
class TextureManager {
public:
    uint16_t loadTexture(std::string file)
    {
        string fullPath = SharedContent.getFullPath(file);
        auto data = SharedContent.loadFile(file);

        auto image_data = ImageData(data.first.get(), data.second);
        return update(filename, data.first.get(), data.second);
    }

    void Load(std::string file)
    {
        uint16_t rid;
        uint16_t cnt;

        auto it = repo_.find(file);
        if (it != repo_.end()) {
            cnt = it->second.cnt;
            rid = it->second.rid;
        } else {
            loadTexture(file)
        }
    }

private:
    std::map<std::string, idCount> repo_;
    std::map<std::string, uint32_t> names_;
}

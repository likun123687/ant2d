#pragma once
#include "audio/sound.h"
#include <unordered_map>
#include <memory>
#include <utils/content.h>

namespace ant2d {
class AudioManager {

    struct SoundCount {
        std::unique_ptr<Sound> ref;
        uint32_t cnt;
    };

public:
    AudioManager();

    Sound* Load(const std::string& key, const std::string& filename, bool stream = false);

    void Unload(const std::string& key);

    Sound* Get(const std::string& key);

    ~AudioManager();

private:
    std::unordered_map<std::string, SoundCount> repo_;
    ma_engine engine_;
};
} // namespace ant2d
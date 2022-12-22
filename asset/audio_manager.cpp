#include "asset/audio_manager.h"
#include <utils/debug.h>

namespace ant2d {

AudioManager::AudioManager()
{
    ma_result result;
    result = ma_engine_init(NULL, &engine_);
    if (result != MA_SUCCESS) {
        Error("init audio engine fail");
    }
}

Sound* AudioManager::Load(const std::string& key, const std::string& filename, bool stream)
{
    Sound* ret = nullptr;
    auto it = repo_.find(key);
    if (it != repo_.end()) {
        it->second.cnt += 1;
        ret = it->second.ref.get();
    } else {
        auto full_path = SharedContent.GetFullPath(filename);
        std::unique_ptr<Sound> snd { new Sound { full_path, &engine_, stream } };
        repo_[key] = SoundCount { std::move(snd), 1 };
        ret = snd.get();
    }

    return ret;
}

void AudioManager::Unload(const std::string& key)
{
    auto it = repo_.find(key);
    if (it != repo_.end()) {
        if (it->second.cnt > 1) {
            it->second.cnt -= 1;
        } else {
            repo_.erase(key);
        }
    }
}

Sound* AudioManager::Get(const std::string& key)
{
    auto it = repo_.find(key);
    if (it != repo_.end()) {
        Info("audio get sound {}", (void*)(&(it->second.ref)));
        return it->second.ref.get();
    }
    return nullptr;
}

AudioManager::~AudioManager()
{
    ma_engine_uninit(&engine_);
}
} // namespace ant2d

#pragma once
#include "third_party/miniaudio/miniaudio.h"
#include <string>
namespace ant2d {
class Sound {
public:
    Sound(const std::string& filepath, ma_engine* engine, bool is_stream = false);

    void Start();

    void Stop();

    void SetVolume(float volume);

    float GetVolume();

    void SetLooping(bool is_looping);
    bool IsLooping();

    ~Sound();

private:
    bool is_stream_;
    ma_sound snd_;
    ma_engine* engine_;
};
} // namespace ant2d

#include <audio/sound.h>
#include <utils/debug.h>
#include <iostream>

namespace ant2d {

Sound::Sound(const std::string& filepath, ma_engine* engine, bool is_stream)
    : engine_(engine)
    , is_stream_(is_stream)
{
    ma_result result;

    ma_uint32 flag = 0;
    if (is_stream) {
        flag |= MA_SOUND_FLAG_STREAM;
    }

    result = ma_sound_init_from_file(engine_, filepath.c_str(), flag, NULL, NULL, &snd_);
    if (result != MA_SUCCESS) {
        Error("load sound error {}--{}", filepath, result);
    }
}

void Sound::Start()
{
    ma_result result;
    result = ma_sound_start(&snd_);
    if (result != MA_SUCCESS) {
        Error("start sound error {}--{}", result, (void*)(&snd_));
    }
}

void Sound::Stop()
{
    ma_sound_stop(&snd_);
}

void Sound::SetVolume(float volume)
{
    ma_sound_set_volume(&snd_, volume);
}

float Sound::GetVolume()
{
    return ma_sound_get_volume(&snd_);
}

void Sound::SetLooping(bool is_looping)
{
    ma_sound_set_looping(&snd_, is_looping);
}

bool Sound::IsLooping()
{
    return ma_sound_is_looping(&snd_);
}

Sound::~Sound()
{
    ma_sound_uninit(&snd_);
}
} // namespace ant2d

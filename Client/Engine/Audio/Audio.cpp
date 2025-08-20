#include "pch.h"
#include "Audio.h"

#include "AudioManager.h"
#include "FMOD/fmod.h"
#include "Misc/StringHelper.h"

Audio::Audio() :
    sound_(nullptr),
    dB_(0)
{
}

Audio::~Audio()
{
    FMOD_Sound_Release(sound_);
}

bool Audio::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);

    std::string path_str = StringHelper::UTF16ToUTF8(kPath);

    if (!meta_data_.IsNull())
    {
        dB_ = meta_data_["dB"].as<int32_t>();
    }

    FMOD_RESULT result = FMOD_System_CreateSound(AudioManager::Get()->fmod_system_, path_str.c_str(), FMOD_DEFAULT, nullptr, &sound_);
    return result == FMOD_OK;
}

void Audio::SetLoop(bool is_loop) const
{
    FMOD_MODE mode;
    FMOD_Sound_GetMode(sound_, &mode);

    if (is_loop)
    {
        mode &= ~FMOD_LOOP_OFF;
        mode |= FMOD_LOOP_NORMAL;
    }
    else
    {
        mode &= ~FMOD_LOOP_NORMAL;
        mode |= FMOD_LOOP_OFF;
    }

    FMOD_Sound_SetMode(sound_, mode);
}

uint32_t Audio::GetLength() const
{
    uint32_t length;
    FMOD_Sound_GetLength(sound_, &length, FMOD_TIMEUNIT_MS);
    return length;
}

AudioFormat Audio::GetFormat() const
{
    AudioFormat format;
    FMOD_Sound_GetFormat(sound_, &format.type, &format.format, &format.channels, &format.bits);
    return format;
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Audio>("Audio")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}

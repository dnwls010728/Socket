#include "pch.h"
#include "Audio.h"

#include "AudioManager.h"
#include "FMOD/fmod.h"

Audio::Audio()
{
}

Audio::~Audio()
{
    FMOD_Sound_Release(sound_);
}

bool Audio::Load(const std::wstring& kPath)
{
    const std::string kFinalPath(kPath.begin(), kPath.end());

    FMOD_RESULT result = FMOD_System_CreateSound(AudioManager::Get()->fmod_system_, kFinalPath.c_str(), FMOD_DEFAULT, nullptr, &sound_);
    return result == FMOD_OK;
}

void Audio::SetLoop(bool is_loop)
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

Type::uint32 Audio::GetLength() const
{
    Type::uint32 length;
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

#include "pch.h"
#include "AudioManager.h"

#include <ranges>

#include "Math/Math.h"
#include "Audio/Audio.h"
#include "Misc/StringHelper.h"

AudioManager::AudioManager() :
    fmod_system_(nullptr),
    channels_(),
    channel_groups_()
{
}

AudioManager::~AudioManager()
{
    FMOD_System_Close(fmod_system_);
    FMOD_System_Release(fmod_system_);
}

bool AudioManager::Init()
{
    FMOD_RESULT result = FMOD_System_Create(&fmod_system_, FMOD_VERSION);
    if (result != FMOD_OK) return false;

    result = FMOD_System_Init(fmod_system_, MAX_CHANNEL_COUNT, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) return false;

    for (int32_t i = 1; i < static_cast<uint8_t>(ChannelGroup::kCount); ++i)
    {
        ChannelGroup group = static_cast<ChannelGroup>(i);

        rttr::enumeration enum_group = rttr::type::get<ChannelGroup>().get_enumeration();
        if (!enum_group.is_valid()) return false;
        
        std::string group_name = enum_group.value_to_name(group).to_string();
        if (group_name.empty()) return false;
        
        result = FMOD_System_CreateChannelGroup(fmod_system_, group_name.c_str(), &channel_groups_[group]);
        if (result != FMOD_OK) return false;
    }
    
    return true;
}

void AudioManager::Tick()
{
    FMOD_System_Update(fmod_system_);
}

void AudioManager::PlayOneShot(const Audio* audio, float volume) const
{
    if (!audio) return;
    
    FMOD_CHANNEL* channel = nullptr;
    FMOD_System_PlaySound(fmod_system_, audio->sound_, nullptr, false, &channel);
    if (channel) FMOD_Channel_SetVolume(channel, volume);
}

void AudioManager::PauseSound(int32_t id) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return;
    FMOD_Channel_SetPaused(channels_[id], true);
}

void AudioManager::ResumeSound(int32_t id) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return;
    FMOD_Channel_SetPaused(channels_[id], false);
}

void AudioManager::StopSound(int32_t id) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return;
    FMOD_Channel_Stop(channels_[id]);
}

void AudioManager::StopAllSounds()
{
    for (const auto& channel : channels_)
    {
        FMOD_Channel_Stop(channel);
    }
}

void AudioManager::SetVolume(int32_t id, int32_t volume) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return;
    volume = Math::Clamp(volume, 0.f, 100.f);
    
    const float final_volume = volume / 100.f;
    FMOD_Channel_SetVolume(channels_[id], final_volume);
}

void AudioManager::SetVolume(ChannelGroup group, int32_t volume) const
{
    if (group == ChannelGroup::kNone) return;

    auto it = channel_groups_.find(group);
    if (it == channel_groups_.end()) return;

    FMOD_CHANNELGROUP* channel_group = it->second;
    if (!channel_group) return;

    volume = Math::Clamp(volume, 0.f, 100.f);
    const float final_volume = volume / 100.f;
    
    FMOD_ChannelGroup_SetVolume(channel_group, final_volume);
}

void AudioManager::SetMasterVolume(int32_t volume) const
{
    FMOD_CHANNELGROUP* master_group = nullptr;
    FMOD_RESULT result = FMOD_System_GetMasterChannelGroup(fmod_system_, &master_group);
    if (result != FMOD_OK) return;

    volume = Math::Clamp(volume, 0.f, 100.f);
    const float final_volume = volume / 100.f;

    FMOD_ChannelGroup_SetVolume(master_group, final_volume);
}

void AudioManager::SetMute(int32_t id, bool is_mute) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return;
    FMOD_Channel_SetMute(channels_[id], is_mute);
}

void AudioManager::SetAllMutes(bool is_mute) const
{
    FMOD_CHANNELGROUP* master_group = nullptr;
    FMOD_RESULT result = FMOD_System_GetMasterChannelGroup(fmod_system_, &master_group);
    if (result != FMOD_OK) return;

    FMOD_ChannelGroup_SetMute(master_group, is_mute);
}

int32_t AudioManager::PlaySound2D(const Audio* audio, ChannelGroup group)
{
    if (!audio) return -1;

    FMOD_CHANNELGROUP* channel_group = nullptr;
    
    auto it = channel_groups_.find(group);
    if (it != channel_groups_.end()) channel_group = it->second;
    
    for (int32_t i = 0; i < MAX_CHANNEL_COUNT; ++i)
    {
        FMOD_CHANNEL* channel = channels_[i];
        FMOD_BOOL is_playing = false;
        FMOD_Channel_IsPlaying(channel, &is_playing);
        
        if (!is_playing)
        {
            FMOD_System_PlaySound(fmod_system_, audio->sound_, channel_group, false, &channels_[i]);
            FMOD_Channel_SetVolumeRamp(channels_[i], true);

            float gain = std::pow(10.f, audio->GetDB() / 20.f);
            FMOD_Channel_SetVolume(channels_[i], gain);
            return i;
        }
    }

    return -1;
}

int32_t AudioManager::GetVolume(int32_t id) const
{
    if (id < 0 || id >= MAX_CHANNEL_COUNT) return 0.f;

    float volume = 0.f;
    FMOD_Channel_GetVolume(channels_[id], &volume);
    return static_cast<int32_t>(volume * 100.f);
}

int32_t AudioManager::GetVolume(ChannelGroup group) const
{
    if (group == ChannelGroup::kNone) return 0.f;

    auto it = channel_groups_.find(group);
    if (it == channel_groups_.end()) return 0.f;

    FMOD_CHANNELGROUP* channel_group = it->second;
    if (!channel_group) return 0.f;

    float volume = 0.f;
    FMOD_ChannelGroup_GetVolume(channel_group, &volume);
    return static_cast<int32_t>(volume * 100.f);
}

int32_t AudioManager::GetMasterVolume() const
{
    FMOD_CHANNELGROUP* master_group = nullptr;
    FMOD_RESULT result = FMOD_System_GetMasterChannelGroup(fmod_system_, &master_group);
    if (result != FMOD_OK) return 0.f;

    float volume = 0.f;
    FMOD_ChannelGroup_GetVolume(master_group, &volume);
    return static_cast<int32_t>(volume * 100.f);
}

#pragma once

#include "ChannelGroup.h"
#include "Singleton.h"
#include "FMOD/fmod.hpp"

#define MAX_CHANNEL_COUNT 4095

class Audio;

class AudioManager : public Singleton<AudioManager>
{
public:
    AudioManager();
    virtual ~AudioManager() override;

    void PlayOneShot(const Audio* audio, float volume = 1.f) const;
    void PauseSound(int32_t id) const;
    void ResumeSound(int32_t id) const;
    void StopSound(int32_t id) const;
    void StopSound(ChannelGroup group) const;
    void StopAllSounds() const;
    void SetVolume(int32_t id, int32_t volume) const;
    void SetVolume(ChannelGroup group, int32_t volume) const;
    void SetMasterVolume(int32_t volume) const;
    void SetMute(int32_t id, bool is_mute) const;
    void SetAllMutes(bool is_mute) const;

    int32_t PlaySound2D(const Audio* audio, ChannelGroup group = ChannelGroup::kNone);

    int32_t GetVolume(int32_t id) const;
    int32_t GetVolume(ChannelGroup group) const;
    int32_t GetMasterVolume() const;

private:
    friend class Audio;
    friend class GameEngine;
    
    bool Init();
    
    void Tick();
    
    class FMOD_SYSTEM* fmod_system_;
    FMOD_CHANNEL* channels_[MAX_CHANNEL_COUNT];

    std::unordered_map<ChannelGroup, FMOD_CHANNELGROUP*> channel_groups_;
    
};

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

    void PlayOneShot(const Audio* audio, float volume = 1.f);
    void PauseSound(int32_t id);
    void ResumeSound(int32_t id);
    void StopSound(int32_t id);
    void StopAllSounds();
    void SetVolume(int32_t id, int32_t volume);
    void SetMute(int32_t id, bool is_mute);
    void SetAllMutes(bool is_mute);

    int32_t PlaySound2D(const Audio* audio, ChannelGroup group = ChannelGroup::kNone);

private:
    friend class Audio;
    friend class GameEngine;
    
    bool Init();
    
    void Tick();
    
    class FMOD_SYSTEM* fmod_system_;
    FMOD_CHANNEL* channels_[MAX_CHANNEL_COUNT];

    std::unordered_map<ChannelGroup, FMOD_CHANNELGROUP*> channel_groups_;
    
};

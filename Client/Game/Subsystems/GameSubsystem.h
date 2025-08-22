#pragma once
#include "Audio/Audio.h"
#include "FMOD/fmod.hpp"
#include "Subsystem/GameInstanceSubsystem.h"

class GameSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(GameSubsystem, GameInstanceSubsystem)
    
public:
    GameSubsystem();
    virtual ~GameSubsystem() override = default;

    virtual void Init() override;

    void PlayBGM(const std::wstring& path);

    static GameSubsystem* Get();

private:
    std::wstring bgm_path_;

    int32_t channel_id_;
    
};

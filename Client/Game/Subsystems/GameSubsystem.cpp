#include "pch.h"
#include "GameSubsystem.h"

#include "GameInstance.h"
#include "Asset/AssetManager.h"
#include "Audio/AudioManager.h"

GameSubsystem::GameSubsystem() :
    channel_id_(-1)
{
}

void GameSubsystem::Init()
{
    GameInstanceSubsystem::Init();
}

void GameSubsystem::PlayBGM(const std::wstring& path)
{
    if (path == bgm_path_) return;
    
    AudioManager* audio_manager = AudioManager::Get();
    if (channel_id_ != -1)
    {
        audio_manager->StopSound(channel_id_);
        channel_id_ = -1;
    }

    if (path.empty())
    {
        bgm_path_.clear();
        return;
    }

    bgm_path_ = path;
    
    Audio* bgm = AssetManager::Get()->Load<Audio>(bgm_path_);
    if (bgm)
    {
        bgm->SetLoop(true);
        
        channel_id_ = audio_manager->PlaySound2D(bgm, ChannelGroup::kBGM);
        if (channel_id_ == -1) bgm_path_.clear();
        return;
    }

    bgm_path_.clear();
}

GameSubsystem* GameSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<GameSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameSubsystem>("GameSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}

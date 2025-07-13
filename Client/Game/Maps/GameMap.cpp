#include "pch.h"
#include "GameMap.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"

GameMap::GameMap(const std::wstring& kName) :
    Level(kName)
{
}

void GameMap::Load()
{
    Level::Load();
    
    InGameReadyPacket packet;
    SessionSubsystem::Get()->SendPacket(packet);

    UI::Get()->ChangeState(UIInGameState::StaticClass());
    
    Audio* bgm = AssetManager::Get()->Load<Audio>(L"Audio\\BGM\\Suspended Moments.mp3");
    bgm->SetLoop(true);
    
    AudioManager::Get()->PlaySound2D(bgm);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameMap>("GameMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

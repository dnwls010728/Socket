#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/GameSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UILoginState.h"

LoginMap::LoginMap(const std::wstring& kName) :
    Level(kName)
{
}

void LoginMap::Load()
{
    Level::Load();
    
    UI::Get()->ChangeState(UILoginState::StaticClass());
    GameSubsystem::Get()->PlayBGM(L"Audio\\BGM\\Dreamscape.mp3");
}

void LoginMap::Unload(EndPlayReason type)
{
    Level::Unload(type);

    AudioManager* audio_manager = AudioManager::Get();
    audio_manager->StopSound(ChannelGroup::kSE);
    audio_manager->StopSound(ChannelGroup::kMobSE);
    audio_manager->StopSound(ChannelGroup::kSkillSE);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<LoginMap>("LoginMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

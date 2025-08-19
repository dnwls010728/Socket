#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
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

    Audio* bgm = AssetManager::Get()->Load<Audio>(L"Audio\\BGM\\Dreamscape.mp3");
    bgm->SetLoop(true);
    
    AudioManager::Get()->PlaySound2D(bgm, ChannelGroup::kBGM);
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

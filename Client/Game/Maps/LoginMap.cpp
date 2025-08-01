#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UILoginState.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/ListBox.h"
#include "UI/Widget/TextBox.h"

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
    
    AudioManager::Get()->PlaySound2D(bgm);
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

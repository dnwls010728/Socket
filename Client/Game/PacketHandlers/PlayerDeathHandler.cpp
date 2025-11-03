#include "pch.h"
#include "PlayerDeathHandler.h"

#include <CustomPacket.h>

#include "PostProcessingSettings.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/Element/UIPopup.h"
#include "Windows/WindowsApplication.h"

bool PlayerDeathHandler::Handle(Net::IPacket* packet)
{
    PlayerDeathPacket* received_packet = dynamic_cast<PlayerDeathPacket*>(packet);
    if (!received_packet) return false;

    PostProcessingSettings::Get()->SetBlurRadius(1.f);
    PostProcessingSettings::Get()->SetVignetteStrength(.5f);
    PostProcessingSettings::Get()->SetGamma(.9f);
    PostProcessingSettings::Get()->SetGrayscale(1.f);

    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\die.mp3");
    if (audio) AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);

    auto player = NetworkSubsystem::Get()->GetPlayer();
    player->SetDead();
    
    UIPopup::PopupParam param;
    param.caption = L"캐릭터가 사망했습니다.\n캐릭터 데이터가 영구적으로 삭제됩니다.";
    param.option = UIPopup::PopupOption::OK;
    param.callback = [&](const std::wstring& text,  UIPopup::PopupOption option)
    {
        if (option == UIPopup::PopupOption::OK)
        {
            // PostProcessingSettings::Get()->SetBlurRadius(0.f);
            // PostProcessingSettings::Get()->SetVignetteStrength(0.f);
            // PostProcessingSettings::Get()->SetGamma(1.f);
            // PostProcessingSettings::Get()->SetGrayscale(0.f);
            //
            // PlayerRespawnPacket respawn_packet;
            // NetworkSubsystem::Get()->SendPacket(respawn_packet);

            WindowsApplication::Get()->QuitApplication();
        }
        
        return true;
    };
    UIPopup::ShowPopup(param);

    return true;
}

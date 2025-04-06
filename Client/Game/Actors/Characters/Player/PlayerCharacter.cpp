#include "pch.h"
#include "PlayerCharacter.h"

#include "GameInstance.h"
#include "Components/PlayerController.h"
#include "Data/PropData.h"
#include "Level/CameraManager.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName)
{
    controller_ = AddComponent<PlayerController>(L"PlayerController");
    
}

void PlayerCharacter::OnAttack()
{
    const PropData* prop_data = GameInstance::Get()->GetPropData(0);
    if (prop_data)
    {
        Logger::Print(L"Prop ID: %d, Name: %s", prop_data->id, prop_data->name.c_str());
    }
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    CameraManager::Get()->SetTarget(GetSharedThis());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerCharacter>("PlayerCharacter")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

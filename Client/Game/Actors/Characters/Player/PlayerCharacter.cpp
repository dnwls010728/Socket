#include "pch.h"
#include "PlayerCharacter.h"

#include "Components/PlayerController.h"
#include "Level/CameraManager.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName)
{
    controller_ = AddComponent<PlayerController>(L"PlayerController");
    
}

void PlayerCharacter::OnAttack()
{
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

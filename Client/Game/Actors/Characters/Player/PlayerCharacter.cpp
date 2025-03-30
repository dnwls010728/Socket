#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Camera.h"
#include "Actors/Bombs/BombBase.h"
#include "Actors/Characters/Player/Components/PlayerController.h"
#include "DirectXTK/Keyboard.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName)
    : CharacterBase(kName)
{
    controller_ = AddComponent<PlayerController>(L"PlayerController");
}

void PlayerCharacter::SpawnBomb()
{
    Logger::Print(L"Spawn Bomb");
    SpawnActor<BombBase>(BombBase::StaticClass(), L"Bomb");
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
    Camera::Get()->SetSize(10.f);
    
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

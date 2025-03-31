#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Camera.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actors/Bombs/BombBase.h"
#include "Actors/Characters/Player/Components/PlayerController.h"
#include "Asset/AssetManager.h"
#include "DirectXTK/Keyboard.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName)
    : CharacterBase(kName)
{
    controller_ = AddComponent<PlayerController>(L"PlayerController");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Player.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Player_0");
    }
}

void PlayerCharacter::SpawnBomb()
{
    Actor* bomb = SpawnActor<Actor>(BombBase::StaticClass(), L"Bomb");
    if (bomb)
    {
        Math::Vector2 position = GetTransform()->GetPosition();
        position.x = std::round(position.x);
        position.y = std::round(position.y);

        bomb->GetTransform()->SetPosition(position);
    }
}

void PlayerCharacter::OnMovement(const Math::Vector2& kMovement, float delta_time)
{
    GetTransform()->Translate(kMovement * 5.f * delta_time);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
    Camera::Get()->SetSize(7.5f);
    
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

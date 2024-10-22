#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Camera.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Resource/ResourceManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName)
{
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Default\\Capsule.png");
    sprite_->SetPPU(256);

    renderer_->SetSprite(sprite_);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);
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

#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Camera.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Resource/ResourceManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName)
{
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Default\\Capsule.png");
    sprite_->SetPPU(256);
    sprite_->Split(1, 1, Sprite::kBottom);

    renderer_->SetSprite(sprite_);
    renderer_->SetColor(Math::Color::Black);

    collider_->SetOffset({0.f, 1.f});
    
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    const int h = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
    velocity_.x = h * 5.f;
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

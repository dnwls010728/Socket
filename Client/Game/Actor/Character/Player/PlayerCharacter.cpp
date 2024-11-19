#include "pch.h"
#include "PlayerCharacter.h"

#include "Logger.h"
#include "Actor/Camera.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Input/Keyboard.h"
#include "Resource/ResourceManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    horizontal_axis_(0),
    move_speed_(2.f)
{
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Character\\PlayerSheet.png");
    if (sprite_) renderer_->SetSprite(sprite_);

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
    
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);
    
    if (horizontal_axis_ != 0)
    {
        renderer_->SetFlipX(horizontal_axis_ < 0);
        rigid_body_->SetLinearVelocityX(horizontal_axis_ * move_speed_);
    }
    
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    horizontal_axis_ = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);

    if (keyboard->GetKeyDown('C'))
    {
        rigid_body_->AddForceY(7.f, ForceMode::kImpulse);
    }
    
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

#include "pch.h"
#include "Mob1.h"

#include <random>

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Asset/AssetManager.h"
#include "State/Mob1Idle.h"
#include "Windows/DX/Sprite.h"

Mob1::Mob1(const std::wstring& kName) :
    MobBase(kName)
{
    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Character\\Mob\\1\\Slime_Spiked_Full.png");
    if (sprite_) renderer_->SetSprite(sprite_, L"Slime_Spiked_Full_0");
    
    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});

    idle_state_ = std::make_shared<Mob1Idle>(state_machine_);
    state_machine_->ChangeState(idle_state_.get());
    
}

void Mob1::BeginPlay()
{
    MobBase::BeginPlay();

    TimerManager::Get()->SetTimer(this, &Mob1::Turn, 1.f, true);
}

void Mob1::Turn()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-1, 1);
    int direction = dis(gen);

    rigid_body_->SetLinearVelocityX(direction * 5.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Mob1>("Mob1")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

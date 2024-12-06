#include "pch.h"
#include "Mob1.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Resource/ResourceManager.h"
#include "State/Mob1Idle.h"
#include "Windows/DX/Sprite.h"

Mob1::Mob1(const std::wstring& kName) :
    MobBase(kName)
{
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Character\\Mob\\1\\Slime_Spiked_Full.png");
    if (sprite_) renderer_->SetSprite(sprite_);
    
    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});

    idle_state_ = std::make_shared<Mob1Idle>(state_machine_);
    state_machine_->ChangeState(idle_state_.get());
    
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

#include "pch.h"
#include "CharacterBase.h"
#include "actor/Component/BoxColliderComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Actors/Character/Components/Controller2DComponent.h"
#include "Actors/States/IdleState.h"
#include "Actors/States/MoveToPlayerState.h"
#include "FSM/Condition.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName),
    state_machine_(nullptr),
    velocity_(Math::Vector2::Zero()),
    gravity_(-20.f)
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetSize({1.f, 1.f});

    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
    state_machine_ = AddComponent<StateMachineComponent>(L"StateMachine");
    
}

void CharacterBase::BeginPlay()
{
    Actor::BeginPlay();
    std::shared_ptr<CharacterBase> this_ptr =  std::static_pointer_cast<CharacterBase>(shared_from_this());
    auto idle = std::make_shared<IdleState>(this_ptr);
    auto move = std::make_shared<MoveToPlayerState>(this_ptr);
    
    state_machine_->AddTransition(idle, move, [this]() {
        return true;
    });
    state_machine_->AddTransition(move, idle, [this]() {
        return false;
    });
    state_machine_->SetState(move);
}


void CharacterBase::PhysicsTick(float delta_time)
{
    Actor::PhysicsTick(delta_time);

    const auto& collisions = controller_->GetCollisions();
    if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;
    
    velocity_.x = movement_input_.x * 5.f;
    velocity_.y += gravity_ * delta_time;
    controller_->Move(velocity_ * delta_time, movement_input_);
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

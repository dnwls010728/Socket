#include "pch.h"
#include "CharacterBase.h"
#include "actor/Component/BoxColliderComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Actors/Character/Components/Controller2DComponent.h"

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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Component/FSM/StateMachine.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    NetworkActor(kName),
    is_dead_(false)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    collider_ = AddComponent<CapsuleColliderComponent>(L"CapsuleCollider");
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody2D");
    rigid_body_->UseAutoMass(false);
    rigid_body_->SetFreezeRotation(true);

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    state_machine_ = AddComponent<StateMachine>(L"StateMachine");
}

void CharacterBase::OnHit()
{
}

void CharacterBase::OnDeath()
{
    is_dead_ = true;
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

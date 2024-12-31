#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Component/FSM/StateMachine.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

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

void CharacterBase::AdjustPosition() const
{
    TransformComponent* transform = GetTransform();

    Bounds collider_bounds = collider_->GetBounds();
    
    Math::Vector2 pivot_offset = collider_->GetOffset();
    pivot_offset.x += .5f;
    pivot_offset.y -= .5f;
    
    Math::Vector2 min_offset = collider_bounds.size * pivot_offset;
    Math::Vector2 max_offset = collider_bounds.size - min_offset;
    
    Math::Vector2 position = transform->GetPosition();
    position.x = Math::Clamp(position.x, -15.f + min_offset.x, 15.f - max_offset.x);
    position.y = Math::Clamp(position.y, -10.f + min_offset.y, 10.f - max_offset.y);

    transform->SetPosition(position);
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

#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Windows/DX/Sprite.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName),
    is_dead_(false)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    collider_ = AddComponent<CircleColliderComponent>(L"CircleCollider");
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody2D");
    rigid_body_->UseAutoMass(false);
    rigid_body_->SetFreezeRotation(true);
    rigid_body_->SetGravityScale(0);

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
}

void CharacterBase::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
    renderer_->SetZOrder(static_cast<int>(std::round(GetTransform()->GetPosition().y)) * -1.f);
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

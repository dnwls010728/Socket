#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    NetworkActor(kName),
    is_dead_(false)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    collider_ = AddComponent<CapsuleColliderComponent>(L"CapsuleCollider");
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody2D");
    rigid_body_->UseAutoMass(false);
    rigid_body_->SetFreezeRotation(true);
}

float CharacterBase::ApplyDamage(CharacterBase* damaged_character, float base_damage, Actor* event_instigator, Actor* damage_causer)
{
    if (IsValid(damaged_character) && base_damage != 0.f)
    {
        return damaged_character->TakeDamage(base_damage, event_instigator, damage_causer);
    }

    return 0.f;
}

float CharacterBase::TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer)
{
    return damage_amount;
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

#include "pch.h"
#include "MobBase.h"

#include "Logger.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"

MobBase::MobBase(const std::wstring& kName) :
    CharacterBase(kName),
    hp_(0.f),
    is_infinite_hp_(false)
{
    SetLayer(ActorLayer::kMob);
}

float MobBase::TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer)
{
    float actual_damage = CharacterBase::TakeDamage(damage_amount, event_instigator, damage_causer);
    if (is_dead_) return 0.f;
    
    if (actual_damage > 0.f)
    {
        OnHit();
        
        if (!is_infinite_hp_)
        {
            hp_ -= actual_damage;

            if (hp_ <= 0.f)
            {
                OnDeath();
            }
        }
    }
    
    Logger::Print(L"Event Instigator: %s, Damage Causer: %s, Current HP: %.f", event_instigator->GetName().c_str(), damage_causer->GetName().c_str(), hp_);
    return actual_damage;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MobBase>("MobBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

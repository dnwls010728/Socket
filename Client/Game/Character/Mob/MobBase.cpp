#include "pch.h"
#include "MobBase.h"

#include "Logger.h"

MobBase::MobBase(const std::wstring& kName) :
    CharacterBase(kName),
    hp_(0.f)
{
    SetLayer(ActorLayer::kMob);
}

float MobBase::TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer)
{
    if (is_dead_) return 0.f;
    
    hp_ -= damage_amount;
    OnHit();
    
    Logger::Print(L"Event Instigator: %s, Damage Causer: %s, Current HP: %.f", event_instigator->GetName().c_str(), damage_causer->GetName().c_str(), hp_);
    
    if (hp_ <= 0.f)
    {
        OnDeath();
    }
    
    return damage_amount;
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

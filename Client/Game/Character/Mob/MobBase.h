#pragma once
#include "Character/CharacterBase.h"

class MobBase : public CharacterBase
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, CharacterBase)
    
public:
    MobBase(const std::wstring& kName);
    virtual ~MobBase() override = default;

protected:
    virtual float TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer) override;
    
};

#pragma once
#include "Character/CharacterBase.h"
#include "Combat/Interface/IDamageable.h"

class MobBase : public CharacterBase, public IDamageable
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, CharacterBase)
    
public:
    MobBase(const std::wstring& kName);
    virtual ~MobBase() override = default;

    virtual void TakeDamage(int damage) override;
    
};

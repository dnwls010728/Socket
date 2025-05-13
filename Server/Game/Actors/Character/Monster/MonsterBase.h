#pragma once
#include "Actors/Character/CharacterBase.h"

class MonsterBase : public CharacterBase
{
public:
    SHADER_CLASS_HELPER(MonsterBase)
    GENERATED_BODY(MonsterBase, CharacterBase)
    
    MonsterBase(std::wstring kName);
protected:
    virtual void Tick(float delta_time) override;
    virtual void PhysicsTick(float delta_time) override;
    
};

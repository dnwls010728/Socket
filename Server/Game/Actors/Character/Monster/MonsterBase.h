#pragma once
#include "Actors/Character/CharacterBase.h"

class MonsterBase : public CharacterBase
{
public:
    SHADER_CLASS_HELPER(MonsterBase)
    GENERATED_BODY(MonsterBase, CharacterBase)
    
    MonsterBase(std::wstring kName);
protected:
    void Tick(float delta_time) override;
};

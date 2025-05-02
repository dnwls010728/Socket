#pragma once
#include "Actors/Characters/CharacterBase.h"

class MonsterBase : public CharacterBase
{
    SHADER_CLASS_HELPER(MonsterBase)
    GENERATED_BODY(MonsterBase, Actor)
    
public:
    MonsterBase(const std::wstring& kName);
    virtual ~MonsterBase() override = default;
    
};

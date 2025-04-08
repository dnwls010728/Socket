#pragma once
#include "Actors/Characters/CharacterBase.h"

class EnemyBase : public CharacterBase
{
    SHADER_CLASS_HELPER(EnemyBase)
    GENERATED_BODY(EnemyBase, Actor)
    
public:
    EnemyBase(const std::wstring& kName);
    virtual ~EnemyBase() override = default;
    
};

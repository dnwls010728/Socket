#pragma once
#include "CharacterBase.h"

class Monster : public CharacterBase
{
public:
    SHADER_CLASS_HELPER(Monster)
    GENERATED_BODY(Monster, CharacterBase)
    
    Monster(std::wstring kName);
protected:
    void Tick(float delta_time) override;
};

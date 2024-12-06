#pragma once
#include "Character/Actor/CharacterBase.h"

class MobBase : public CharacterBase
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase)
    
public:
    MobBase(const std::wstring& kName);
    virtual ~MobBase() override = default;

protected:
    class StateMachine* state_machine_;
    
};

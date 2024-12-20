#pragma once
#include "Character/Mob/MobBase.h"

class Dummy : public MobBase
{
    SHADER_CLASS_HELPER(Dummy)
    GENERATED_BODY(Dummy, MobBase)
    
public:
    Dummy(const std::wstring& kName);
    virtual ~Dummy() override = default;

private:
    class Sprite* sprite_;
    
};

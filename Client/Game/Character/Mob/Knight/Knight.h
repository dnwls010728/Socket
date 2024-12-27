#pragma once
#include "Character/Mob/MobBase.h"

class Knight : public MobBase
{
    SHADER_CLASS_HELPER(Knight)
    GENERATED_BODY(Knight, MobBase)
    
public:
    Knight(const std::wstring& kName);
    virtual ~Knight() override = default;

private:
    class AnimationPack* animation_pack_;

    std::shared_ptr<class KnightIdle> idle_state_;
    
};

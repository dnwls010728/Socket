#pragma once
#include "Character/Mob/MobBase.h"

class Dummy : public MobBase
{
    SHADER_CLASS_HELPER(Dummy)
    GENERATED_BODY(Dummy, MobBase)
    
public:
    Dummy(const std::wstring& kName);
    virtual ~Dummy() override = default;

protected:
    virtual void Tick(float delta_time) override;
    virtual void OnHit() override;

private:
    class Sprite* sprite_;
    class AnimationPack* animation_pack_;
    
};

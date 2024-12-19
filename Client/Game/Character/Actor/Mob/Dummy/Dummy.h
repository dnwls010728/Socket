#pragma once
#include "Character/Actor/Mob/MobBase.h"

class Dummy : public MobBase
{
    SHADER_CLASS_HELPER(Dummy)
    GENERATED_BODY(Dummy, MobBase)
    
public:
    Dummy(const std::wstring& kName);
    virtual ~Dummy() override = default;

    virtual void OnDamaged(float damage) override;

protected:
    virtual void Tick(float delta_time) override;

private:
    class Sprite* sprite_;
    class AnimationPack* animation_pack_;

    class AnimatorComponent* animator_;
    
};

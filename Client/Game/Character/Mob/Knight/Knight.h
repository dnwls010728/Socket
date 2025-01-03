#pragma once
#include "Character/Mob/MobBase.h"

namespace BT
{
    class BehaviourTree;
}

class Knight : public MobBase
{
    SHADER_CLASS_HELPER(Knight)
    GENERATED_BODY(Knight, MobBase)
    
public:
    Knight(const std::wstring& kName);
    virtual ~Knight() override = default;

    bool M();

protected:
    virtual void Tick(float delta_time) override;
    
    virtual void OnHit() override;
    virtual void OnDeath() override;

private:
    class AnimationPack* animation_pack_;

    std::shared_ptr<BT::BehaviourTree> behaviour_tree_;
    
};

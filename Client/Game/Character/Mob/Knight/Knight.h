#pragma once
#include "Character/Blackboard/Blackboard.h"
#include "Character/Mob/MobBase.h"

namespace BT
{
    class BehaviorTree;
}

class Knight : public MobBase
{
    SHADER_CLASS_HELPER(Knight)
    GENERATED_BODY(Knight, MobBase)
    
public:
    Knight(const std::wstring& kName);
    virtual ~Knight() override = default;

protected:
    virtual void Tick(float delta_time) override;
    
    virtual void OnHit() override;
    virtual void OnDeath() override;

private:
    class AnimationPack* animation_pack_;

    std::shared_ptr<Blackboard::Blackboard> blackboard_;
    std::shared_ptr<BT::BehaviorTree> behavior_tree_;
    
};

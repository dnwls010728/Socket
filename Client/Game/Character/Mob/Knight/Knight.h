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
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
    virtual void OnHit() override;
    virtual void OnDeath() override;

private:
    void SetRandomLocation();
    
    class AnimationPack* animation_pack_;

    class ContextSteering* context_steering_;

    std::shared_ptr<Blackboard::Blackboard> blackboard_;
    std::shared_ptr<BT::BehaviorTree> behavior_tree_;

    Blackboard::BlackboardKey target_key_;
    Blackboard::BlackboardKey location_key_;
    
};

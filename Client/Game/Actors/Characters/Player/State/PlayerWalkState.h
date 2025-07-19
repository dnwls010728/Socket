#pragma once
#include "PlayerStateBase.h"

class PlayerWalkState : public PlayerStateBase
{
public:
    PlayerWalkState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator);
    virtual ~PlayerWalkState() override = default;

    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
};

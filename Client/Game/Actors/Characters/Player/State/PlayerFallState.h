#pragma once
#include "PlayerStateBase.h"

class PlayerFallState : public PlayerStateBase
{
public:
    PlayerFallState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator);
    virtual ~PlayerFallState() override = default;

    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
};

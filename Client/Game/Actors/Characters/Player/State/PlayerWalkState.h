#pragma once
#include "PlayerState.h"

class PlayerWalkState : public PlayerState
{
public:
    PlayerWalkState(const std::shared_ptr<PlayerCharacter>& player_character);
    virtual ~PlayerWalkState() override = default;

    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Exit() override;
    
};

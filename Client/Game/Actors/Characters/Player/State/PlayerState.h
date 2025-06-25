#pragma once
#include "FSM/IState.h"

class PlayerCharacter;

class PlayerState : public FSM::IState
{
public:
    PlayerState(const std::shared_ptr<PlayerCharacter>& player_character);
    virtual ~PlayerState() override = default;
    
    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    virtual void Exit() override;

protected:
    std::shared_ptr<PlayerCharacter> player_character_;
    
};

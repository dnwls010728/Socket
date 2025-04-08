#pragma once
#include "Actors/Characters/States/BaseState.h"

class PlayerWalkState : public BaseState
{
public:
    PlayerWalkState(const std::shared_ptr<CharacterBase>& kCharacter);
    virtual ~PlayerWalkState() override = default;

    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Exit() override;
    
};

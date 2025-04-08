#pragma once
#include "FSM/IState.h"

class CharacterBase;

class BaseState : public FSM::IState
{
public:
    BaseState(const std::shared_ptr<CharacterBase>& kCharacter);
    virtual ~BaseState() override = default;
    
    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    virtual void Exit() override;

protected:
    std::weak_ptr<CharacterBase> character_;
    
};

#pragma once
#include "Character/Component/FSM/State.h"

class Mob1Idle : public State
{
    SHADER_CLASS_HELPER(Mob1Idle)
    GENERATED_BODY(Mob1Idle, State)
    
public:
    Mob1Idle(StateMachine* state_machine);
    virtual ~Mob1Idle() override = default;

protected:
    virtual void Enter() override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;
    
};

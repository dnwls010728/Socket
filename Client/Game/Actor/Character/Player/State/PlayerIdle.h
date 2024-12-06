#pragma once
#include "Character/Component/FSM/State.h"

class PlayerIdle : public State
{
    SHADER_CLASS_HELPER(PlayerIdle)
    GENERATED_BODY(PlayerIdle, State)
    
public:
    PlayerIdle(StateMachine* state_machine);
    virtual ~PlayerIdle() override = default;

protected:
    virtual void Enter() override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;
    
};

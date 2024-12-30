#pragma once
#include "Character/Component/FSM/State.h"
#include "Time/TimerManager.h"

class KnightIdle : public State
{
    SHADER_CLASS_HELPER(KnightIdle)
    GENERATED_BODY(KnightIdle, State)
    
public:
    KnightIdle(StateMachine* state_machine);
    virtual ~KnightIdle() override = default;
    
protected:
    virtual void Enter() override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;

private:
    class Knight* character_;
    class AnimatorComponent* animator_;

    TimerHandle timer_handle_;
    
};

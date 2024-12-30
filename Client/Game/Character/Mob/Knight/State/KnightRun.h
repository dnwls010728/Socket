#pragma once
#include "Character/Component/FSM/State.h"
#include "Time/TimerManager.h"

class KnightRun : public State
{
    SHADER_CLASS_HELPER(KnightRun)
    GENERATED_BODY(KnightRun, State)
    
public:
    KnightRun(StateMachine* state_machine);
    virtual ~KnightRun() override = default;
    
protected:
    virtual void Enter() override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;
    
private:
    class Knight* character_;
    class AnimatorComponent* animator_;

    TimerHandle timer_handle_;
    
};

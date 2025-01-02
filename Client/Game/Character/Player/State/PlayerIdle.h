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
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    virtual void Exit() override;

private:
    class PlayerCharacter* character_;
    class AnimatorComponent* animator_;
    
};

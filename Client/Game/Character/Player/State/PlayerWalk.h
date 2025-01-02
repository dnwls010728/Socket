#pragma once
#include "Character/Component/FSM/State.h"

class PlayerWalk : public State
{
    SHADER_CLASS_HELPER(PlayerWalk)
    GENERATED_BODY(PlayerWalk, State)
    
public:
    PlayerWalk(StateMachine* state_machine);
    virtual ~PlayerWalk() override = default;

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

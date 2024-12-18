#pragma once
#include "Character/Component/FSM/State.h"

class PlayerLocomotion : public State
{
    SHADER_CLASS_HELPER(PlayerLocomotion)
    GENERATED_BODY(PlayerLocomotion, State)
    
public:
    PlayerLocomotion(StateMachine* state_machine);
    virtual ~PlayerLocomotion() override = default;
    
protected:
    virtual void Enter() override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;

private:
    class PlayerCharacter* player_;
    
    class AnimatorComponent* animator_;
    
};

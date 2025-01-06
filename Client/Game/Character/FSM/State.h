#pragma once
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class StateMachine;

class State : public std::enable_shared_from_this<State>
{
    SHADER_CLASS_HELPER(State)
    GENERATED_BODY(State)
    
public:
    State(StateMachine* state_machine);
    virtual ~State() = default;

protected:
    friend class StateMachine;
    
    virtual void Enter() = 0;
    virtual void PhysicsTick(float delta_time) = 0;
    virtual void Tick(float delta_time) = 0;
    virtual void PostTick(float delta_time) = 0;
    virtual void Exit() = 0;

protected:
    StateMachine* state_machine_;
    
};

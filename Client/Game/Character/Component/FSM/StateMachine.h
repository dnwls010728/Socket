#pragma once
#include "Actor/Component/ActorComponent.h"

class State;

class StateMachine : public ActorComponent
{
    SHADER_CLASS_HELPER(StateMachine)
    GENERATED_BODY(StateMachine, ActorComponent)
    
public:
    StateMachine(Actor* owner, const std::wstring& kName);
    virtual ~StateMachine() override = default;

    void ChangeState(State* new_state);

protected:
    virtual void PhysicsTickComponent(float delta_time) override;
    virtual void TickComponent(float delta_time) override;
    virtual void PostTickComponent(float delta_time) override;

private:
    State* current_state_;
    
};

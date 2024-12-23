#include "pch.h"
#include "StateMachine.h"

#include "Actor/Actor.h"
#include "State.h"

StateMachine::StateMachine(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    current_state_(nullptr)
{
}

void StateMachine::ChangeState(State* new_state)
{
    if (current_state_) current_state_->Exit();
    current_state_ = new_state;
    if (current_state_) current_state_->Enter();
}

void StateMachine::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    if (current_state_) current_state_->Tick(delta_time);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<StateMachine>("StateMachine")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

#include "pch.h"
#include "StateMachineComponent.h"

#include "FSM/Condition.h"

StateMachineComponent::StateMachineComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    state_machine_(nullptr)
{
}

void StateMachineComponent::SetState(const std::shared_ptr<FSM::IState>& kState)
{
    if (state_machine_) state_machine_->SetState(kState);
}

void StateMachineComponent::ChangeState(const std::shared_ptr<FSM::IState>& kState)
{
    if (state_machine_) state_machine_->ChangeState(kState);
}

void StateMachineComponent::AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, bool(* func)())
{
    if (state_machine_) state_machine_->AddTransition(kFrom, kTo, std::make_shared<FSM::Condition>(func));
}

void StateMachineComponent::AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, bool(* func)())
{
    if (state_machine_) state_machine_->AddAnyTransition(kTo, std::make_shared<FSM::Condition>(func));
}

void StateMachineComponent::AddState(const std::shared_ptr<FSM::IState>& kState) const
{
    if (state_machine_) state_machine_->GetOrAddNode(kState);
}

void StateMachineComponent::SetState(const std::shared_ptr<FSM::IState>& kState) const
{
    if (state_machine_) state_machine_->SetState(kState);
}

void StateMachineComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    state_machine_ = std::make_unique<FSM::StateMachine>();
    
}

void StateMachineComponent::PhysicsTickComponent(float delta_time)
{
    ActorComponent::PhysicsTickComponent(delta_time);

    if (state_machine_) state_machine_->PhysicsTick(delta_time);
}

void StateMachineComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    if (state_machine_) state_machine_->Tick(delta_time);
}

void StateMachineComponent::PostTickComponent(float delta_time)
{
    ActorComponent::PostTickComponent(delta_time);

    if (state_machine_) state_machine_->PostTick(delta_time);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<StateMachineComponent>("StateMachineComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

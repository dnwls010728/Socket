#include "pch.h"
#include "StateMachine.h"

#include "IState.h"
#include "Transition.h"

FSM::StateMachine::StateNode::StateNode(const std::shared_ptr<IState>& kState) :
    state_(kState),
    transitions_()
{
}

void FSM::StateMachine::StateNode::AddTransition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition)
{
    transitions_.emplace(std::make_shared<Transition>(kTo, kCondition));
}

FSM::StateMachine::StateMachine() :
    current_state_(nullptr),
    nodes_(),
    any_transitions_()
{
}

void FSM::StateMachine::PhysicsTick(float delta_time)
{
    if (current_state_ && current_state_->GetState()) current_state_->GetState()->PhysicsTick(delta_time);
}

void FSM::StateMachine::Tick(float delta_time)
{
    const std::shared_ptr<Transition> transition = GetTransition();
    if (transition) ChangeState(transition->GetTo());

    if (current_state_ && current_state_->GetState()) current_state_->GetState()->Tick(delta_time);
}

void FSM::StateMachine::PostTick(float delta_time)
{
    if (current_state_ && current_state_->GetState()) current_state_->GetState()->PostTick(delta_time);
}

void FSM::StateMachine::SetState(const std::shared_ptr<IState>& kState)
{
    current_state_ = nodes_[typeid(*kState)];
    if (current_state_ && current_state_->GetState()) current_state_->GetState()->Enter();
}

void FSM::StateMachine::ChangeState(const std::shared_ptr<IState>& kState)
{
    if (kState == current_state_->GetState()) return;

    std::shared_ptr<IState> previous_state = current_state_->GetState();
    std::shared_ptr<IState> next_state = nodes_[typeid(*kState)]->GetState();

    if (previous_state) previous_state->Exit();
    if (next_state) next_state->Enter();
    current_state_ = nodes_[typeid(*kState)];
}

void FSM::StateMachine::AddTransition(const std::shared_ptr<IState>& kFrom, const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition)
{
    GetOrAddNode(kFrom)->AddTransition(GetOrAddNode(kTo)->GetState(), kCondition);
}

void FSM::StateMachine::AddAnyTransition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition)
{
    any_transitions_.emplace(std::make_shared<Transition>(GetOrAddNode(kTo)->GetState(), kCondition));
}

std::shared_ptr<FSM::Transition> FSM::StateMachine::GetTransition() const
{
    for (const auto& kTransition : any_transitions_)
    {
        if (kTransition->CheckCondition()) return kTransition;
    }

    if (current_state_)
    {
        for (const auto& kTransition : current_state_->GetTransitions())
        {
            if (kTransition->CheckCondition()) return kTransition;
        }
    }
    
    return nullptr;
}

std::shared_ptr<FSM::StateMachine::StateNode> FSM::StateMachine::GetOrAddNode(const std::shared_ptr<IState>& kState)
{
    std::shared_ptr<StateNode> node = nullptr;
    
    const auto it = nodes_.find(typeid(*kState));
    if (it != nodes_.end()) node = it->second;

    if (!node)
    {
        node = std::make_shared<StateNode>(kState);
        nodes_.emplace(typeid(*kState), node);
    }

    return node;
}

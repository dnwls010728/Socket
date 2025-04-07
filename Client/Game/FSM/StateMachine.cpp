#include "pch.h"
#include "StateMachine.h"

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
    current_state_(nullptr)
{
}

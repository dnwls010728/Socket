#include "pch.h"
#include "Transition.h"

#include "Condition.h"

FSM::Transition::Transition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition) :
    to_(kTo),
    condition_(kCondition)
{
}

bool FSM::Transition::CheckCondition() const
{
    return condition_->Evaluate();
}

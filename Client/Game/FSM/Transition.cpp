#include "pch.h"
#include "Transition.h"

FSM::Transition::Transition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition) :
    to_(kTo),
    condition_(kCondition)
{
}

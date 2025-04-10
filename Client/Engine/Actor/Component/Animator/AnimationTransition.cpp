#include "pch.h"
#include "AnimationTransition.h"

#include "AnimationCondition.h"

Transition::Transition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition) :
    to_(kTo),
    condition_(kCondition)
{
}

bool Transition::CheckCondition(AnimatorComponent* animator)
{
    return condition_->Evaluate(animator);
}

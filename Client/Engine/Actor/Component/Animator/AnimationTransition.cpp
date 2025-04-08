#include "pch.h"
#include "AnimationTransition.h"

#include "AnimationCondition.h"

AnimationTransition::AnimationTransition(const std::wstring& kTo, const std::shared_ptr<AnimationCondition>& kCondition) :
    to_(kTo),
    condition_(kCondition)
{
}

bool AnimationTransition::CheckCondition(AnimatorComponent* animator)
{
    return condition_->Evaluate(animator);
}

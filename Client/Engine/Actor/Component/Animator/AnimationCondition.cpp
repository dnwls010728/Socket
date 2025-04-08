#include "pch.h"
#include "AnimationCondition.h"

AnimationCondition::AnimationCondition(bool(* func)(AnimatorComponent*)) :
    function_(func)
{
}

bool AnimationCondition::Evaluate(AnimatorComponent* animator)
{
    return function_(animator);
}

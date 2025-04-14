#include "pch.h"
#include "AnimationCondition.h"

Condition::Condition(bool(* func)(AnimatorComponent*)) :
    function_(func)
{
}

bool Condition::Evaluate(AnimatorComponent* animator)
{
    return function_(animator);
}

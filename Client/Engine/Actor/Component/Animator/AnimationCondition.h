#pragma once
#include "AnimatorComponent.h"

class AnimationCondition
{
public:
    template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(AnimatorComponent*)>, std::decay_t<F>>>>
    AnimationCondition(F&& func);

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    AnimationCondition(M* target, bool (M::*func)(AnimatorComponent*));

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    AnimationCondition(M* target, bool (M::*func)(AnimatorComponent*) const);

    AnimationCondition(bool (*func)(AnimatorComponent*));

    ~AnimationCondition() = default;

    bool Evaluate(AnimatorComponent* animator);

protected:
    Function<bool(AnimatorComponent*)> function_;
};

template <typename F, typename>
AnimationCondition::AnimationCondition(F&& func) :
    function_(std::forward<F>(func))
{
}

template <typename M, typename>
AnimationCondition::AnimationCondition(M* target, bool (M::*func)(AnimatorComponent*)) :
    function_(target, func)
{
}

template <typename M, typename>
AnimationCondition::AnimationCondition(M* target, bool (M::*func)(AnimatorComponent*) const) :
    function_(target, func)
{
}

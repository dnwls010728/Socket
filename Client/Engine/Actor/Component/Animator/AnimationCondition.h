#pragma once
#include "AnimatorComponent.h"

class Condition
{
public:
    template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(AnimatorComponent*)>, std::decay_t<F>>>>
    Condition(F&& func);

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    Condition(M* target, bool (M::*func)(AnimatorComponent*));

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    Condition(M* target, bool (M::*func)(AnimatorComponent*) const);

    Condition(bool (*func)(AnimatorComponent*));

    ~Condition() = default;

    bool Evaluate(AnimatorComponent* animator);

protected:
    Function<bool(AnimatorComponent*)> function_;
};

template <typename F, typename>
Condition::Condition(F&& func) :
    function_(std::forward<F>(func))
{
}

template <typename M, typename>
Condition::Condition(M* target, bool (M::*func)(AnimatorComponent*)) :
    function_(target, func)
{
}

template <typename M, typename>
Condition::Condition(M* target, bool (M::*func)(AnimatorComponent*) const) :
    function_(target, func)
{
}

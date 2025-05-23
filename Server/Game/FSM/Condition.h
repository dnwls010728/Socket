#pragma once
#include "Misc/Function.h"

namespace FSM
{
    class Condition
    {
    public:
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
        Condition(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Condition(M* target, bool(M::*func)(void));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Condition(M* target, bool(M::*func)(void) const);

        Condition(bool(*func)(void));

        ~Condition() = default;

        bool Evaluate();

    protected:
        Function<bool(void)> function_;
    
    };

    template <typename F, typename>
    Condition::Condition(F&& func) :
        function_(std::forward<F>(func))
    {
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)()) :
        function_(target, func)
    {
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)() const) :
        function_(target, func)
    {
    }
}

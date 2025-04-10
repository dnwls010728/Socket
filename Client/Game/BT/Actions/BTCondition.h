#pragma once
#include "IStrategy.h"

namespace BT
{
    class Condition : public IStrategy
    {
    public:
        template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
        Condition(F&& func);

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Condition(M* target, bool (M::*func)(void));

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        Condition(M* target, bool (M::*func)(void) const);

        Condition(bool (*func)(void));
        
        virtual ~Condition() override = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        Function<bool(void)> condition_;
        
    };

    template <typename F, typename>
    Condition::Condition(F&& func) :
        condition_(std::forward<F>(func))
    {
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)()) :
        condition_(target, func)
    {
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)() const) :
        condition_(target, func)
    {
    }
}

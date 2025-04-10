#pragma once
#include "IStrategy.h"

namespace BT
{
    class ConditionStrategy : public IStrategy
    {
    public:
        template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
        ConditionStrategy(F&& func);

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        ConditionStrategy(M* target, bool (M::*func)(void));

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        ConditionStrategy(M* target, bool (M::*func)(void) const);

        ConditionStrategy(bool (*func)(void));
        
        virtual ~ConditionStrategy() override = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        Function<bool(void)> condition_;
        
    };

    template <typename F, typename>
    ConditionStrategy::ConditionStrategy(F&& func) :
        condition_(std::forward<F>(func))
    {
    }

    template <typename M, typename>
    ConditionStrategy::ConditionStrategy(M* target, bool(M::* func)()) :
        condition_(target, func)
    {
    }

    template <typename M, typename>
    ConditionStrategy::ConditionStrategy(M* target, bool(M::* func)() const) :
        condition_(target, func)
    {
    }
}

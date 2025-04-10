#pragma once
#include "IStrategy.h"

namespace BT
{
    class ActionStrategy : public IStrategy
    {
    public:
        template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(void)>, std::decay_t<F>>>>
        ActionStrategy(F&& func);

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        ActionStrategy(M* target, void (M::*func)(void));

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        ActionStrategy(M* target, void (M::*func)(void) const);

        ActionStrategy(void (*func)(void));

        virtual ~ActionStrategy() override = default;

        virtual Node::Status TickNode(float delta_time) override;
        
    private:
        Function<void(void)> action_;
    
    };

    template <typename F, typename>
    ActionStrategy::ActionStrategy(F&& func) :
        action_(std::forward<F>(func))
    {
    }

    template <typename M, typename>
    ActionStrategy::ActionStrategy(M* target, void(M::* func)()) :
        action_(target, func)
    {
    }

    template <typename M, typename>
    ActionStrategy::ActionStrategy(M* target, void(M::* func)() const) :
        action_(target, func)
    {
    }
}

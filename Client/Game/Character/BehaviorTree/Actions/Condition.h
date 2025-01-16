#pragma once
#include "IStrategy.h"
#include "Misc/Function.h"

namespace BT
{
    class Condition : public IStrategy
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

        virtual Node::Status TickNode(float delta_time) override;
    
    private:
        Function<bool(void)> func_;
    
    };

    template <typename F, typename>
    Condition::Condition(F&& func) :
        func_([](){return false;})
    {
        func_ = std::forward<F>(func);
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)()) :
        func_([](){return false;})
    {
        func_ = {target, func};
    }

    template <typename M, typename>
    Condition::Condition(M* target, bool(M::* func)() const) :
        func_([](){return false;})
    {
        func_ = {target, func};
    }
}

#pragma once
#include <memory>

namespace FSM
{
    class IState;
    class Condition;
}

namespace FSM
{
    class Transition
    {
    public:
        Transition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition);
        ~Transition() = default;

        FORCEINLINE const std::shared_ptr<IState>& GetTo() const { return to_; }
        
        bool CheckCondition() const;

    protected:
        std::shared_ptr<IState> to_;
        std::shared_ptr<Condition> condition_;
    
    };
}

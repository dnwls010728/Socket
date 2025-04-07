#pragma once

namespace FSM
{
    class IState;
    class Condition;
    class Transition;
}

namespace FSM
{
    class StateMachine
    {
    private:
        class StateNode
        {
        public:
            StateNode(const std::shared_ptr<IState>& kState);
            ~StateNode() = default;

            void AddTransition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition);

        private:
            std::shared_ptr<IState> state_;
            std::unordered_set<std::shared_ptr<Transition>> transitions_;
        };
    
    public:
        StateMachine();
        ~StateMachine() = default;

    private:
        StateNode current_state_;
    
    };
}

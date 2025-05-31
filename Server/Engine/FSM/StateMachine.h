#pragma once
#include <typeindex>

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

            FORCEINLINE const std::shared_ptr<IState>& GetState() const { return state_; }
            FORCEINLINE const std::unordered_set<std::shared_ptr<Transition>>& GetTransitions() const { return transitions_; }

        private:
            std::shared_ptr<IState> state_;
            std::unordered_set<std::shared_ptr<Transition>> transitions_;
        };
    
    public:
        StateMachine();
        ~StateMachine() = default;

        void PhysicsTick(float delta_time);
        void Tick(float delta_time);
        void PostTick(float delta_time);
        void SetState(std::shared_ptr<IState> kState);
        void ChangeState(std::shared_ptr<IState> kState);
        void AddTransition(const std::shared_ptr<IState>& kFrom, const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition);
        void AddAnyTransition(const std::shared_ptr<IState>& kTo, const std::shared_ptr<Condition>& kCondition);

        std::shared_ptr<Transition> GetTransition() const;

        std::shared_ptr<StateNode> GetOrAddNode(const std::shared_ptr<IState>& kState);

    private:
        std::shared_ptr<StateNode> current_state_;
        std::unordered_map<std::type_index, std::shared_ptr<StateNode>> nodes_;
        std::unordered_set<std::shared_ptr<Transition>> any_transitions_;
    
    };
}

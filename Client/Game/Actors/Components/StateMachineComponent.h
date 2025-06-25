#pragma once
#include "Actor/Component/ActorComponent.h"
#include "FSM/StateMachine.h"

class StateMachineComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(StateMachineComponent)
    GENERATED_BODY(StateMachineComponent, ActorComponent)
    
public:
    StateMachineComponent(Actor* owner, const std::wstring& kName);
    virtual ~StateMachineComponent() override = default;

    void SetState(const std::shared_ptr<FSM::IState>& kState);
    void ChangeState(const std::shared_ptr<FSM::IState>& kState);
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
    void AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::*func)(void));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::*func)(void) const);

    void AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, bool(*func)(void));
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
    void AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::*func)(void));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::*func)(void) const);

    void AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, bool(*func)(void));
    void AddState(const std::shared_ptr<FSM::IState>& kState) const;
    void SetState(const std::shared_ptr<FSM::IState>& kState) const;

protected:
    virtual void InitializeComponent() override;
    virtual void PhysicsTickComponent(float delta_time) override;
    virtual void TickComponent(float delta_time) override;
    virtual void PostTickComponent(float delta_time) override;

private:
    std::unique_ptr<FSM::StateMachine> state_machine_;
    
};

template <typename F, typename>
void StateMachineComponent::AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, F&& func)
{
    if (state_machine_) state_machine_->AddTransition(kFrom, kTo, std::make_shared<FSM::Condition>(std::forward<F>(func)));
}

template <typename M, typename>
void StateMachineComponent::AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::* func)())
{
    if (state_machine_) state_machine_->AddTransition(kFrom, kTo, std::make_shared<FSM::Condition>(target, func));
}

template <typename M, typename>
void StateMachineComponent::AddTransition(const std::shared_ptr<FSM::IState>& kFrom, const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::* func)() const)
{
    if (state_machine_) state_machine_->AddTransition(kFrom, kTo, std::make_shared<FSM::Condition>(target, func));
}

template <typename F, typename>
void StateMachineComponent::AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, F&& func)
{
    if (state_machine_) state_machine_->AddAnyTransition(kTo, std::make_shared<FSM::Condition>(std::forward<F>(func)));
}

template <typename M, typename>
void StateMachineComponent::AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, M* target, bool(M::* func)())
{
    if (state_machine_) state_machine_->AddAnyTransition(kTo, std::make_shared<FSM::Condition>(target, func));
}

template <typename M, typename>
void StateMachineComponent::AddAnyTransition(const std::shared_ptr<FSM::IState>& kTo, M* target,
    bool(M::* func)() const)
{
    if (state_machine_) state_machine_->AddAnyTransition(kTo, std::make_shared<FSM::Condition>(target, func));
}

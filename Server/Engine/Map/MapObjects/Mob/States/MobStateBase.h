#pragma once
#include "FSM/IState.h"

class Mob;

namespace FSM
{
    class StateMachine;
}

class MobStateBase : public FSM::IState
{
public:
    MobStateBase(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine);
    virtual ~MobStateBase() override = default;
    
    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;

protected:
    std::weak_ptr<Mob> owner_;
    FSM::StateMachine& state_machine_;
    
};

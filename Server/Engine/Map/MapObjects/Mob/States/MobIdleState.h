#pragma once
#include "MobStateBase.h"

class MobIdleState : public MobStateBase
{
public:
    MobIdleState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine);
    virtual ~MobIdleState() override = default;

    virtual void Enter() override;
    virtual void Tick(float delta_time) override;

protected:
    float transition_timer_;
    
};

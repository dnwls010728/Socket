#pragma once
#include "MobStateBase.h"

class MobWalkState : public MobStateBase
{
public:
    MobWalkState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine);
    virtual ~MobWalkState() override = default;

    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void Exit() override;

protected:
    float transition_timer_;

    bool is_right_;
    
};

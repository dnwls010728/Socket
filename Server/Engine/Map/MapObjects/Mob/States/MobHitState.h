#pragma once
#include "MobStateBase.h"

class MobHitState : public MobStateBase
{
public:
    MobHitState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine);
    virtual ~MobHitState() override = default;

    virtual void Enter() override;
    virtual void Tick(float delta_time) override;

private:
    float transition_timer_;
    
};

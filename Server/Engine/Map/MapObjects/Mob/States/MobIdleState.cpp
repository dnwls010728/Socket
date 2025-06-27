#include "pch.h"
#include "MobIdleState.h"

#include "MobWalkState.h"
#include "FSM/StateMachine.h"
#include "Map/MapObjects/Mob/Mob.h"

MobIdleState::MobIdleState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine) :
    MobStateBase(owner, state_machine),
    transition_timer_(0.f)
{
}

void MobIdleState::Enter()
{
    MobStateBase::Enter();
}

void MobIdleState::Tick(float delta_time)
{
    MobStateBase::Tick(delta_time);

    transition_timer_ += delta_time;
    if (transition_timer_ >= 1.6f)
    {
        if (auto owner = owner_.lock()) state_machine_.ChangeState(owner->GetWalkState());
        transition_timer_ -= 1.6f;
    }
    
}

#include "pch.h"
#include "MobHitState.h"

#include "MobIdleState.h"
#include "FSM/StateMachine.h"
#include "Map/MapObjects/Mob/Mob.h"

MobHitState::MobHitState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine) :
    MobStateBase(owner, state_machine),
    transition_timer_(0.f)
{
}

void MobHitState::Enter()
{
    MobStateBase::Enter();
    
    if (auto owner = owner_.lock())
    {
        owner->SetState(MobState::kHit);
    }
}

void MobHitState::Tick(float delta_time)
{
    MobStateBase::Tick(delta_time);

    transition_timer_ += delta_time;
    if (transition_timer_ >= .625f)
    {
        if (auto owner = owner_.lock()) state_machine_.ChangeState(owner->GetIdleState());
        transition_timer_ -= .625f;
    }
    
}

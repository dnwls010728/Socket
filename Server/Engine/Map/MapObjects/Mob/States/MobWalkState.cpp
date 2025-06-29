#include "pch.h"
#include "MobWalkState.h"

#include "MobIdleState.h"
#include "FSM/StateMachine.h"
#include "Map/MapObjects/Mob/Mob.h"
#include "Math/Math.h"

MobWalkState::MobWalkState(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine) :
    MobStateBase(owner, state_machine),
    transition_timer_(0.f),
    is_right_(false)
{
}

void MobWalkState::Enter()
{
    MobStateBase::Enter();

    is_right_ = Math::RandRange(0, 1);
    if (auto owner = owner_.lock())
    {
        owner->SetState(MobState::kWalk);
        owner->SetFlipped(!is_right_);
    }
    
}

void MobWalkState::PhysicsTick(float delta_time)
{
    MobStateBase::PhysicsTick(delta_time);

    float velocity_x = 2.f * (is_right_ ? 1.f : -1.f);
    if (auto owner = owner_.lock())
    {
        owner->SetVelocityX(velocity_x);
    }
    
}

void MobWalkState::Tick(float delta_time)
{
    MobStateBase::Tick(delta_time);
    
    transition_timer_ += delta_time;
    if (transition_timer_ >= 1.6f)
    {
        if (auto owner = owner_.lock())
        {
            int32_t rand_state = Math::RandRange(0, 1);
            if (rand_state == 0) state_machine_.ChangeState(owner->GetIdleState());
            else if (rand_state == 1) state_machine_.ChangeState(owner->GetWalkState());
        }
        
        transition_timer_ -= 1.6f;
    }
    
}

void MobWalkState::Exit()
{
    MobStateBase::Exit();

    if (auto owner = owner_.lock())
    {
        owner->SetVelocityX(0.f);
    }
    
}

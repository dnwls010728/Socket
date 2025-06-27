#include "pch.h"
#include "MobStateBase.h"

MobStateBase::MobStateBase(const std::shared_ptr<Mob>& owner, FSM::StateMachine& state_machine) :
    owner_(owner),
    state_machine_(state_machine)
{
}

void MobStateBase::Enter()
{
}

void MobStateBase::PhysicsTick(float delta_time)
{
}

void MobStateBase::Tick(float delta_time)
{
}

void MobStateBase::Exit()
{
}

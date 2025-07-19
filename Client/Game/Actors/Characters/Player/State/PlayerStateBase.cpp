#include "pch.h"
#include "PlayerStateBase.h"

PlayerStateBase::PlayerStateBase(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator) :
    owner_(owner),
    animator_(animator)
{
}

void PlayerStateBase::Enter()
{
}

void PlayerStateBase::PhysicsTick(float delta_time)
{
}

void PlayerStateBase::Tick(float delta_time)
{
}

void PlayerStateBase::PostTick(float delta_time)
{
}

void PlayerStateBase::Exit()
{
}

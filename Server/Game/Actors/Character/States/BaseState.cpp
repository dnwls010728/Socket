#include "pch.h"
#include "BaseState.h"

BaseState::BaseState(const std::shared_ptr<CharacterBase>& kCharacter) :
    character_(kCharacter)
{
}

void BaseState::Enter()
{
}

void BaseState::PhysicsTick(float delta_time)
{
}

void BaseState::Tick(float delta_time)
{
}

void BaseState::PostTick(float delta_time)
{
}

void BaseState::Exit()
{
}

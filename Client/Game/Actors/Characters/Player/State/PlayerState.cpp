#include "pch.h"
#include "PlayerState.h"

PlayerState::PlayerState(const std::shared_ptr<PlayerCharacter>& player_character) :
    player_character_(player_character)
{
}

void PlayerState::Enter()
{
}

void PlayerState::PhysicsTick(float delta_time)
{
}

void PlayerState::Tick(float delta_time)
{
}

void PlayerState::PostTick(float delta_time)
{
}

void PlayerState::Exit()
{
}

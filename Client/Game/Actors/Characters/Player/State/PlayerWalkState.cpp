#include "pch.h"
#include "PlayerWalkState.h"

PlayerWalkState::PlayerWalkState(const std::shared_ptr<PlayerCharacter>& player_character) :
    PlayerState(player_character)
{
}

void PlayerWalkState::Enter()
{
    PlayerState::Enter();

    Logger::Print(L"PlayerWalkState::Enter");
}

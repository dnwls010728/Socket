#include "pch.h"
#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState(const std::shared_ptr<PlayerCharacter>& player_character) :
    PlayerState(player_character)
{
}

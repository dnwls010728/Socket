#include "pch.h"
#include "PlayerFallState.h"

#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerFallState::PlayerFallState(const std::shared_ptr<PlayerCharacter>& player_character) :
    PlayerState(player_character)
{
}

void PlayerFallState::Enter()
{
    PlayerState::Enter();
    
    const Math::Vector2& movement_input = player_character_->GetMovementInput();
    player_character_->SetVelocityX(movement_input.x * 5.f);
}

void PlayerFallState::Exit()
{
    PlayerState::Exit();

    player_character_->SetVelocityX(0.f);
}

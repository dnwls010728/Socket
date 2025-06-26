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
    player_character_->SetVelocityX(player_character_->GetVelocityX());
}

void PlayerFallState::PhysicsTick(float delta_time)
{
    PlayerState::PhysicsTick(delta_time);
    
    const Math::Vector2& movement_input = player_character_->GetMovementInput();
    Math::Vector2 velocity = player_character_->GetVelocity();
    if (movement_input.x < 0.f && velocity.x > 0.f) velocity.x -= .1f;
    else if (movement_input.x > 0.f && velocity.x < 0.f) velocity.x += .1f;
    player_character_->SetVelocityX(velocity.x);
}

void PlayerFallState::Exit()
{
    PlayerState::Exit();

    player_character_->SetVelocityX(0.f);
}

#include "pch.h"
#include "PlayerWalkState.h"

#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerWalkState::PlayerWalkState(const std::shared_ptr<PlayerCharacter>& player_character) :
    PlayerState(player_character)
{
}

void PlayerWalkState::Enter()
{
    PlayerState::Enter();

    Logger::Print(L"PlayerWalkState::Enter");
}

void PlayerWalkState::PhysicsTick(float delta_time)
{
    PlayerState::PhysicsTick(delta_time);

    const Math::Vector2& movement_input = player_character_->GetMovementInput();
    player_character_->SetVelocityX(movement_input.x * 5.f);
}

void PlayerWalkState::Exit()
{
    PlayerState::Exit();

    if (IsValid(player_character_)) player_character_->SetVelocityX(0.f);
}

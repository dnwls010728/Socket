#include "pch.h"
#include "PlayerWalkState.h"

#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerWalkState::PlayerWalkState(const std::shared_ptr<CharacterBase>& kCharacter) :
    BaseState(kCharacter)
{
}

void PlayerWalkState::Enter()
{
    BaseState::Enter();
    
    std::shared_ptr<CharacterBase> character = character_.lock();
    if (IsValid(character.get()))
    {
        std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(character);
        if (player_character) player_character->ClearLinearVelocity();
    }
}

void PlayerWalkState::PhysicsTick(float delta_time)
{
    BaseState::PhysicsTick(delta_time);

    std::shared_ptr<CharacterBase> character = character_.lock();
    if (IsValid(character.get()))
    {
        std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(character);
        if (player_character) player_character->OnMovement();
    }
}

void PlayerWalkState::Exit()
{
    BaseState::Exit();
    
    std::shared_ptr<CharacterBase> character = character_.lock();
    if (IsValid(character.get()))
    {
        std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(character);
        if (player_character) player_character->ClearLinearVelocity();
    }
}

#include "pch.h"
#include "PlayerIdleState.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerIdleState::PlayerIdleState(const std::shared_ptr<PlayerCharacter>& player_character) :
    PlayerState(player_character)
{
}

void PlayerIdleState::Enter()
{
    PlayerState::Enter();

    std::shared_ptr<AnimatorComponent> animator = player_character_->GetAnimator();
    if (animator) animator->PlayAnimation(L"Idle");
}

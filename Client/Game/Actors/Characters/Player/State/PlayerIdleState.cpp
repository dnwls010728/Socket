#include "pch.h"
#include "PlayerIdleState.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerIdleState::PlayerIdleState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator) :
    PlayerStateBase(owner, animator)
{
}

void PlayerIdleState::Enter()
{
    PlayerStateBase::Enter();

    if (auto owner = owner_.lock())
    {
        owner->SetVelocityX(0.f);
    }

    if (auto animator = animator_.lock())
    {
        animator->PlayAnimation(L"Idle");
    }
    
}

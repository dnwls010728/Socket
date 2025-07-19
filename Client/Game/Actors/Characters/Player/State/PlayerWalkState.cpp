#include "pch.h"
#include "PlayerWalkState.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerWalkState::PlayerWalkState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator) :
    PlayerStateBase(owner, animator)
{
}

void PlayerWalkState::Enter()
{
    PlayerStateBase::Enter();

    if (auto animator = animator_.lock())
    {
        animator->PlayAnimation(L"Walk");
    }
    
}

void PlayerWalkState::PhysicsTick(float delta_time)
{
    PlayerStateBase::PhysicsTick(delta_time);

    if (auto owner = owner_.lock())
    {
        owner->SetVelocityX(owner->GetMoveAxisX() * 3.75f);
    }
    
}

void PlayerWalkState::PostTick(float delta_time)
{
    PlayerStateBase::PostTick(delta_time);

    if (auto owner = owner_.lock())
    {
        owner->UpdateFlip();
    }
    
}

#include "pch.h"
#include "PlayerDeathState.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerDeathState::PlayerDeathState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator) :
    PlayerStateBase(owner, animator)
{
}

void PlayerDeathState::Enter()
{
    PlayerStateBase::Enter();
    
    if (auto player = owner_.lock())
    {
        player->SetVelocityX(0.f);
    }
    
    if (auto animator = animator_.lock())
    {
        animator->PlayAnimation(L"Die");
    }
}

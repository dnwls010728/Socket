#include "pch.h"
#include "PlayerFallState.h"

#include "Actors/Characters/Player/PlayerCharacter.h"

PlayerFallState::PlayerFallState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator) :
    PlayerStateBase(owner, animator)
{
}

void PlayerFallState::PhysicsTick(float delta_time)
{
    PlayerStateBase::PhysicsTick(delta_time);

    if (auto owner = owner_.lock())
    {
        float move_axis_x = owner->GetMoveAxisX();
        float velocity_x = owner->GetVelocityX();

        if (move_axis_x < 0.f && velocity_x > 0.f) velocity_x -= .1f;
        else if (move_axis_x > 0.f && velocity_x < 0.f) velocity_x += .1f;

        owner->SetVelocityX(velocity_x);
    }
    
}

void PlayerFallState::PostTick(float delta_time)
{
    PlayerStateBase::PostTick(delta_time);
    
    if (auto owner = owner_.lock())
    {
        owner->UpdateFlip();
    }
    
}

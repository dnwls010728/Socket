#include "pch.h"
#include "PlayerLocomotion.h"

#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"

PlayerLocomotion::PlayerLocomotion(StateMachine* state_machine) :
    State(state_machine),
    player_(nullptr),
    animator_(nullptr)
{
}

void PlayerLocomotion::Enter()
{
    player_ = dynamic_cast<PlayerCharacter*>(state_machine_->GetOwner());
    if (IsValid(player_))
    {
        animator_ = player_->GetAnimator();
        animator_->PlayAnimation(L"Idle");
    }
}

void PlayerLocomotion::Tick(float delta_time)
{
    if (player_->GetHorizontalAxis() != 0) animator_->PlayAnimation(L"Run");
    else animator_->PlayAnimation(L"Idle");
}

void PlayerLocomotion::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerLocomotion>("PlayerLocomotion")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

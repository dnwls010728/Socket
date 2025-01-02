#include "pch.h"
#include "PlayerIdle.h"

#include "PlayerWalk.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Player/PlayerCharacter.h"

PlayerIdle::PlayerIdle(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr)
{
}

void PlayerIdle::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<PlayerCharacter*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Idle");
}

void PlayerIdle::PhysicsTick(float delta_time)
{
}

void PlayerIdle::Tick(float delta_time)
{
    if (character_->GetHAxis() != 0)
    {
        state_machine_->ChangeState(character_->GetWalkState());
    }
}

void PlayerIdle::PostTick(float delta_time)
{
}

void PlayerIdle::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerIdle>("PlayerIdle")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

#include "pch.h"
#include "PlayerWalk.h"

#include "PlayerIdle.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Player/PlayerCharacter.h"

PlayerWalk::PlayerWalk(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr)
{
}

void PlayerWalk::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<PlayerCharacter*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }
    
    animator_->PlayAnimation(L"Walk");
}

void PlayerWalk::PhysicsTick(float delta_time)
{
}

void PlayerWalk::Tick(float delta_time)
{
    if (character_->GetHAxis() == 0)
    {
        state_machine_->ChangeState(character_->GetIdleState());
    }
}

void PlayerWalk::PostTick(float delta_time)
{
}

void PlayerWalk::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerWalk>("PlayerWalk")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

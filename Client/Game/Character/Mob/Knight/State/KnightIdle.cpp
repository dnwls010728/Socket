#include "pch.h"
#include "KnightIdle.h"

#include "KnightRun.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/CharacterBase.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Mob/Knight/Knight.h"

KnightIdle::KnightIdle(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr)
{
}

void KnightIdle::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<Knight*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Idle");

    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        state_machine_->ChangeState(character_->GetRunState());
    }, 2.f);
}

void KnightIdle::Tick(float delta_time)
{
}

void KnightIdle::Exit()
{
}

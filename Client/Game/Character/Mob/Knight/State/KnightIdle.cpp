#include "pch.h"
#include "KnightIdle.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/CharacterBase.h"
#include "Character/Component/FSM/StateMachine.h"

KnightIdle::KnightIdle(StateMachine* state_machine) :
    State(state_machine),
    animator_(nullptr)
{
}

void KnightIdle::Enter()
{
    if (!animator_)
    {
        CharacterBase* character = dynamic_cast<CharacterBase*>(state_machine_->GetOwner());
        if (IsValid(character))
        {
            animator_ = character->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Idle");
}

void KnightIdle::Tick(float delta_time)
{
}

void KnightIdle::Exit()
{
}

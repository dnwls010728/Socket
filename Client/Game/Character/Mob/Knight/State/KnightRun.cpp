#include "pch.h"
#include "KnightRun.h"

#include "KnightIdle.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Mob/Knight/Knight.h"

KnightRun::KnightRun(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr)
{
}

void KnightRun::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<Knight*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Run");
}

void KnightRun::Tick(float delta_time)
{
}

void KnightRun::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<KnightRun>("KnightRun")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

#include "pch.h"
#include "KnightHit.h"

#include "KnightIdle.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Mob/Knight/Knight.h"

KnightHit::KnightHit(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr),
    timer_handle_()
{
}

void KnightHit::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<Knight*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Hit");

    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        state_machine_->ChangeState(character_->GetIdleState());
    }, 1.f);
}

void KnightHit::PhysicsTick(float delta_time)
{
}

void KnightHit::Tick(float delta_time)
{
}

void KnightHit::PostTick(float delta_time)
{
}

void KnightHit::Exit()
{
    TimerManager::Get()->ClearTimer(timer_handle_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<KnightHit>("KnightHit")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

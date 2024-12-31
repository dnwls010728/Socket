#include "pch.h"
#include "KnightRun.h"

#include "KnightIdle.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Mob/Knight/Knight.h"
#include "Math/Math.h"

KnightRun::KnightRun(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr),
    direction_(1)
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

    float duration = Math::RandRange(1.f, 3.f);

    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        state_machine_->ChangeState(character_->GetIdleState());
    }, duration);

    direction_ = Math::RandRange(0, 1) == 0 ? -1 : 1;
    character_->GetRenderer()->SetFlipX(direction_ == -1);
}

void KnightRun::PhysicsTick(float delta_time)
{
    character_->GetRigidBody()->SetLinearVelocityX(direction_ * 2.f);
}

void KnightRun::Tick(float delta_time)
{
}

void KnightRun::PostTick(float delta_time)
{
}

void KnightRun::Exit()
{
    TimerManager::Get()->ClearTimer(timer_handle_);
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

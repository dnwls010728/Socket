#include "pch.h"
#include "KnightDeath.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Character/Mob/Knight/Knight.h"
#include "Math/Math.h"

KnightDeath::KnightDeath(StateMachine* state_machine) :
    State(state_machine),
    character_(nullptr),
    animator_(nullptr),
    timer_handle_(),
    alpha_(1.f)
{
}

void KnightDeath::Enter()
{
    if (!IsValid(character_))
    {
        character_ = dynamic_cast<Knight*>(state_machine_->GetOwner());
        if (IsValid(character_))
        {
            animator_ = character_->GetAnimator();
        }
    }

    animator_->PlayAnimation(L"Death");
}

void KnightDeath::PhysicsTick(float delta_time)
{
}

void KnightDeath::Tick(float delta_time)
{
    alpha_ = Math::Lerp(alpha_, 0.f, 2.f * delta_time);
    if (alpha_ <= 0.01f)
    {
        character_->Destroy();
    }
    else
    {
        character_->GetRenderer()->SetColor({255, 255, 255, static_cast<uint8_t>(255 * alpha_)});
    }
}

void KnightDeath::PostTick(float delta_time)
{
}

void KnightDeath::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<KnightDeath>("KnightDeath")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

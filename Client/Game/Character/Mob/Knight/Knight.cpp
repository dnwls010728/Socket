#include "pch.h"
#include "Knight.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/Component/FSM/StateMachine.h"
#include "State/KnightDeath.h"
#include "State/KnightHit.h"
#include "State/KnightIdle.h"
#include "State/KnightRun.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetOffset({0.f, 1.f});

    animator_->SetAnimationPack(animation_pack_);

    {
        idle_state_ = std::make_shared<KnightIdle>(state_machine_);
        run_state_ = std::make_shared<KnightRun>(state_machine_);
        hit_state_ = std::make_shared<KnightHit>(state_machine_);
        death_state_ = std::make_shared<KnightDeath>(state_machine_);
    }

    state_machine_->ChangeState(idle_state_.get());

    hp_ = 100.f;
}

void Knight::OnHit()
{
    MobBase::OnHit();

    if (state_machine_->IsCurrentState(hit_state_.get())) return;

    state_machine_->ChangeState(hit_state_.get());
}

void Knight::OnDeath()
{
    MobBase::OnDeath();
    
    if (state_machine_->IsCurrentState(death_state_.get())) return;

    state_machine_->ChangeState(death_state_.get());
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Knight>("Knight")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

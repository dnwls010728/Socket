#include "pch.h"
#include "Knight.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/BehaviourTree/BehaviourTree.h"
#include "Character/BehaviourTree/Condition.h"
#include "Character/BehaviourTree/Leaf.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetSize({.5f, .5f});
    collider_->SetOffset({0.f, .5f});

    animator_->SetAnimationPack(animation_pack_);
    animator_->PlayAnimation(L"Idle");

    // hp_ = 100.f;
    is_infinite_hp_ = true;

    behaviour_tree_ = std::make_shared<BT::BehaviourTree>(L"Knight");
}

void Knight::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    behaviour_tree_->Process();
}

void Knight::OnHit()
{
    MobBase::OnHit();

}

void Knight::OnDeath()
{
    MobBase::OnDeath();
    
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

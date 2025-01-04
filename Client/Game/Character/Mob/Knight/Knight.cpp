#include "pch.h"
#include "Knight.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "BehaviourTree/IdleStrategy.h"
#include "BehaviourTree/WalkStrategy.h"
#include "Character/BehaviorTree/BehaviorTree.h"
#include "Character/BehaviorTree/Leaf.h"
#include "Character/BehaviorTree/Sequence.h"
#include "Character/BehaviorTree/Wait.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetSize({.5f, .5f});
    collider_->SetOffset({0.f, .5f});

    animator_->SetAnimationPack(animation_pack_);

    // hp_ = 100.f;
    is_infinite_hp_ = true;

    behavior_tree_ = std::make_shared<BT::BehaviorTree>(L"Knight");
    
    std::shared_ptr<BT::Sequence> actions = std::make_shared<BT::Sequence>(L"Agent Logic");
    actions->AddChild(std::make_shared<BT::Leaf>(L"Idle", std::make_shared<BT::IdleStrategy>(animator_)));
    actions->AddChild(std::make_shared<BT::Wait>(L"Wait", 1.f));
    actions->AddChild(std::make_shared<BT::Leaf>(L"Walk", std::make_shared<BT::WalkStrategy>(animator_)));
    actions->AddChild(std::make_shared<BT::Wait>(L"Wait", 1.f));
    
    behavior_tree_->AddChild(actions);
}

void Knight::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    behavior_tree_->Process();
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

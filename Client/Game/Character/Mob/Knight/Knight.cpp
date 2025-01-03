#include "pch.h"
#include "Knight.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "BehaviourTree/IdleStrategy.h"
#include "BehaviourTree/WalkStrategy.h"
#include "Character/BehaviourTree/BehaviourTree.h"
#include "Character/BehaviourTree/Leaf.h"
#include "Character/BehaviourTree/Sequence.h"
#include "Character/BehaviourTree/Wait.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetSize({.5f, .5f});
    collider_->SetOffset({0.f, .5f});

    animator_->SetAnimationPack(animation_pack_);

    // hp_ = 100.f;
    is_infinite_hp_ = true;

    behaviour_tree_ = std::make_shared<BT::BehaviourTree>(L"Knight");
    
    std::shared_ptr<BT::Sequence> actions = std::make_shared<BT::Sequence>(L"Agent Logic");

    std::shared_ptr<BT::IdleStrategy> idle_strategy = std::make_shared<BT::IdleStrategy>(animator_);
    std::shared_ptr<BT::Leaf> idle = std::make_shared<BT::Leaf>(L"Wander", idle_strategy);
    actions->AddChild(idle);

    std::shared_ptr<BT::Wait> wait = std::make_shared<BT::Wait>(L"Wait", 1.f);
    actions->AddChild(wait);

    std::shared_ptr<BT::WalkStrategy> walk_strategy = std::make_shared<BT::WalkStrategy>(animator_);
    std::shared_ptr<BT::Leaf> walk = std::make_shared<BT::Leaf>(L"Walk", walk_strategy);
    actions->AddChild(walk);

    std::shared_ptr<BT::Wait> wait2 = std::make_shared<BT::Wait>(L"Wait", 1.f);
    actions->AddChild(wait2);
    
    behaviour_tree_->AddChild(actions);
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

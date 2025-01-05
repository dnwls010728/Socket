#include "pch.h"
#include "Knight.h"

#include "Logger.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "BehaviourTree/PatrolStrategy.h"
#include "Character/BehaviorTree/BehaviorTree.h"
#include "Character/BehaviorTree/Leaf.h"
#include "Character/BehaviorTree/Selector.h"
#include "Character/Blackboard/Blackboard.h"

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
    
    behavior_tree_ = std::make_shared<BT::BehaviorTree>(L"Knight");
    
    {
        std::shared_ptr<BT::Selector> actions = std::make_shared<BT::Selector>(L"Agent Logic");

        {
            std::shared_ptr<BT::Leaf> patrol = std::make_shared<BT::Leaf>(L"Patrol", std::make_shared<BT::PatrolStrategy>(animator_, rigid_body_));
            actions->AddChild(patrol);
        }
        
        behavior_tree_->AddChild(actions);
    }

    blackboard_ = std::make_shared<Blackboard::Blackboard>();
    temp_key_ = blackboard_->GetOrRegisterKey(L"TempKey");
    blackboard_->SetValue(temp_key_, 10);

    int a = 0;
    if (blackboard_->TryGetValue(temp_key_, a))
    {
    }

    int t = 0;
    
}

void Knight::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    behavior_tree_->TickNode(delta_time);
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

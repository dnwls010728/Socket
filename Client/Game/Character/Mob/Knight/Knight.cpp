#include "pch.h"
#include "Knight.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/BehaviorTree/BehaviorTree.h"
#include "Character/BehaviorTree/Leaf.h"
#include "Character/BehaviorTree/Selector.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/ContextSteering/ContextSteering.h"
#include "Character/Player/PlayerCharacter.h"
#include "Input/Mouse.h"
#include "Windows/DX/Renderer.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName),
    target_(nullptr)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetRadius(.25f);
    collider_->SetOffset({0.f, .25f});

    animator_->SetAnimationPack(animation_pack_);
    animator_->PlayAnimation(L"Idle");

    // hp_ = 100.f;
    is_infinite_hp_ = true;

    context_steering_ = AddComponent<ContextSteering>(L"Context Steering");
    
    blackboard_ = std::make_shared<Blackboard::Blackboard>();
    
    behavior_tree_ = std::make_shared<BT::BehaviorTree>(L"Knight");
    
    {
        std::shared_ptr<BT::Selector> actions = std::make_shared<BT::Selector>(L"Agent Logic");
        behavior_tree_->AddChild(actions);
    }
    
}

void Knight::BeginPlay()
{
    MobBase::BeginPlay();

    target_ = World::Get()->GetActor(PlayerCharacter::StaticClass());

}

void Knight::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    behavior_tree_->TickNode(delta_time);
    
    if (IsValid(target_) && context_steering_->IsStopped())
    {
        context_steering_->SetDestination(target_->GetTransform()->GetPosition());
    }
    
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

#include "pch.h"
#include "Knight.h"

#include "DebugDrawHelper.h"
#include "Logger.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/Animation.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/BehaviorTree/ActionStrategy.h"
#include "Character/BehaviorTree/BehaviorTree.h"
#include "Character/BehaviorTree/Leaf.h"
#include "Character/BehaviorTree/Selector.h"
#include "Character/BehaviorTree/Sequence.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/ContextSteering/ContextSteering.h"
#include "Character/Mob/BehaviorTree/CheckDetector.h"
#include "Character/Mob/BehaviorTree/MoveToTarget.h"
#include "Character/Player/PlayerCharacter.h"
#include "Input/Mouse.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "Windows/DX/Renderer.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName)
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
    
    Blackboard::BlackboardKey self_key = blackboard_->GetOrRegisterKey(L"Self");
    blackboard_->SetValue(self_key, static_cast<Actor*>(this));
    
    target_key_ = blackboard_->GetOrRegisterKey(L"Target");
    blackboard_->SetValue(target_key_, nullptr);
    
    behavior_tree_ = std::make_shared<BT::BehaviorTree>(L"Knight");
    
    {
        std::shared_ptr<BT::Sequence> actions = std::make_shared<BT::Sequence>(L"Agent Logic");

        {
            std::shared_ptr<BT::Leaf> check_detector = std::make_shared<BT::Leaf>(L"Check Detector", std::make_shared<BT::CheckDetector>(blackboard_.get()));
            actions->AddChild(check_detector);

            std::shared_ptr<BT::Leaf> move_to_target = std::make_shared<BT::Leaf>(L"Move To Target", std::make_shared<BT::MoveToTarget>(blackboard_.get()));
            actions->AddChild(move_to_target);
        }
        
        behavior_tree_->AddChild(actions);
    }
    
}

void Knight::BeginPlay()
{
    MobBase::BeginPlay();

}

void Knight::PhysicsTick(float delta_time)
{
    MobBase::PhysicsTick(delta_time);

    Math::Vector2 position = GetTransform()->GetPosition();

    Actor* target = nullptr;
    if (Physics2D::OverlapCircle(position, 5.f, &target, static_cast<Type::uint16>(ActorLayer::kPlayer)))
    {
        Math::Vector2 direction = (target->GetTransform()->GetPosition() - GetTransform()->GetPosition()).Normalized();

        HitResult hit_result;
        if (Physics2D::RayCast(hit_result, position, direction, 5.f, static_cast<Type::uint16>(ActorLayer::kBlock)))
        {
            target = nullptr;
        }
    }
    
    blackboard_->SetValue(target_key_, target);
}

void Knight::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    behavior_tree_->TickNode(delta_time);

    Math::Vector2 velocity = rigid_body_->GetLinearVelocity();
    
    if (rigid_body_->GetLinearVelocity() != Math::Vector2::Zero())
    {
        renderer_->SetFlipX(rigid_body_->GetLinearVelocity().x < 0.f);

        if (animator_->GetCurrentAnimation()->GetName() != L"Run")
        {
            animator_->PlayAnimation(L"Run");
        }
    }
    else
    {
        if (animator_->GetCurrentAnimation()->GetName() != L"Idle")
        {
            animator_->PlayAnimation(L"Idle");
        }
    }
    
}

void Knight::PostTick(float delta_time)
{
    MobBase::PostTick(delta_time);

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

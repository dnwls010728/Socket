#include "pch.h"
#include "Knight.h"

#include "DebugDrawHelper.h"
#include "Logger.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/BehaviorTree/BehaviorTree.h"
#include "Character/BehaviorTree/Leaf.h"
#include "Character/BehaviorTree/Selector.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/ContextSteering/IObstacle.h"
#include "Character/Player/PlayerCharacter.h"
#include "Input/Mouse.h"
#include "Physics/Physics2D.h"
#include "Windows/DX/Renderer.h"

Knight::Knight(const std::wstring& kName) :
    MobBase(kName),
    directions_(),
    obstacles_(),
    danger_(8),
    interest_(8),
    target_(nullptr)
{
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Knight\\KnightSheet.png.animpack");

    collider_->SetSize({.5f, .5f});
    collider_->SetOffset({0.f, .5f});

    animator_->SetAnimationPack(animation_pack_);
    animator_->PlayAnimation(L"Idle");

    // hp_ = 100.f;
    is_infinite_hp_ = true;
    
    blackboard_ = std::make_shared<Blackboard::Blackboard>();
    
    behavior_tree_ = std::make_shared<BT::BehaviorTree>(L"Knight");
    
    {
        std::shared_ptr<BT::Selector> actions = std::make_shared<BT::Selector>(L"Agent Logic");
        behavior_tree_->AddChild(actions);
    }

    directions_.push_back(Math::Vector2(0.f, 1.f).Normalized());
    directions_.push_back(Math::Vector2(1.f, 1.f).Normalized());
    directions_.push_back(Math::Vector2(1.f, 0.f).Normalized());
    directions_.push_back(Math::Vector2(1.f, -1.f).Normalized());
    directions_.push_back(Math::Vector2(0.f, -1.f).Normalized());
    directions_.push_back(Math::Vector2(-1.f, -1.f).Normalized());
    directions_.push_back(Math::Vector2(-1.f, 0.f).Normalized());
    directions_.push_back(Math::Vector2(-1.f, 1.f).Normalized());
    
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

    Mouse* mouse = Mouse::Get();

    Math::Vector2 world = Renderer::Get()->ConvertScreenToWorld(mouse->GetMousePosition());
    GetTransform()->SetPosition(world);
    
    DetectObstacle();
    GetDangerSteering();
    GetSeekSteering();

    for (int i = 0; i < danger_.size(); ++i)
    {
        DebugDrawHelper::Get()->DrawRay(GetTransform()->GetPosition(), directions_[i] * danger_[i] * 2.f, {255, 0, 0, 255});
    }

    for (int i = 0; i < interest_.size(); ++i)
    {
        DebugDrawHelper::Get()->DrawRay(GetTransform()->GetPosition(), directions_[i] * interest_[i] * 2.f, {0, 255, 0, 255});
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

void Knight::DetectObstacle()
{
    DebugDrawHelper::Get()->DrawCircle(GetTransform()->GetPosition(), 2.f, {255, 0, 0, 255});
    
    std::vector<Actor*> out_actors;
    bool is_hit = Physics2D::OverlapCircleAll(GetTransform()->GetPosition(), 2.f, out_actors, static_cast<Type::uint16>(ActorLayer::kBlock));

    if (is_hit)
    {
        obstacles_ = out_actors;
        Logger::Print(L"Obstacle Detected");
    }
}

void Knight::GetDangerSteering()
{
    Math::Vector2 position = GetTransform()->GetPosition();

    danger_.assign(danger_.size(), 0.f);
    
    for (const auto& kActor : obstacles_)
    {
        IObstacle* obstacle = dynamic_cast<IObstacle*>(kActor);
        if (obstacle)
        {
            Math::Vector2 direction = obstacle->GetCollider()->ClosestPoint(position) - position;
            float distance = direction.Magnitude();

            float weight = distance <= .6f ? 1.f : (2.f - distance) / 2.f;

            Math::Vector2 direction_normalized = direction.Normalized();

            for (int i = 0; i < directions_.size(); ++i)
            {
                float result = Math::Vector2::Dot(direction_normalized, directions_[i]);
                float steering = result * weight;

                if (steering > danger_[i])
                {
                    danger_[i] = steering;
                }
            }
        }
    }
}

void Knight::GetSeekSteering()
{
    interest_.assign(danger_.size(), 0.f);
    
    Math::Vector2 position = GetTransform()->GetPosition();
    Math::Vector2 target_position = target_->GetTransform()->GetPosition();

    Math::Vector2 direction = target_position - position;
    for (int i = 0; i < directions_.size(); ++i)
    {
        float result = Math::Vector2::Dot(direction.Normalized(), directions_[i]);

        if (result > 0.f)
        {
            float steering = result;

            if (steering > interest_[i])
            {
                interest_[i] = steering;
            }
        }
    }
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

#include "pch.h"
#include "ContextSteeringComponent.h"

#include "DebugDrawHelper.h"
#include "IObstacle.h"
#include "Actor/Actor.h"
#include "Actor/Component/ColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"

std::vector<Math::Vector2> ContextSteeringComponent::directions_ = {
    Math::Vector2(0.f, 1.f).Normalized(),
    Math::Vector2(1.f, 1.f).Normalized(),
    Math::Vector2(1.f, 0.f).Normalized(),
    Math::Vector2(1.f, -1.f).Normalized(),
    Math::Vector2(0.f, -1.f).Normalized(),
    Math::Vector2(-1.f, -1.f).Normalized(),
    Math::Vector2(-1.f, 0.f).Normalized(),
    Math::Vector2(-1.f, 1.f).Normalized()
};

ContextSteeringComponent::ContextSteeringComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    rigid_body_(nullptr),
    speed_(3.5f),
    stopping_distance_(0.f),
    is_stopped_(true),
    destination_(Math::Vector2::Zero()),
    direction_(Math::Vector2::Zero()),
    obstacles_(),
    obstacle_layer_(ActorLayer::kBlock)
{
}

void ContextSteeringComponent::SetDestination(const Math::Vector2& destination)
{
    destination_ = destination;
    is_stopped_ = false;
}

void ContextSteeringComponent::Stop()
{
    is_stopped_ = true;
    if (rigid_body_) rigid_body_->SetLinearVelocity(Math::Vector2::Zero());
}

bool ContextSteeringComponent::IsComplete()
{
    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();
    float distance = Math::Vector2::Distance(position, destination_);
    return distance <= stopping_distance_;
}

void ContextSteeringComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    ActorComponent* component = GetOwner()->GetComponent(RigidBody2DComponent::StaticClass());
    if (component) rigid_body_ = static_cast<RigidBody2DComponent*>(component);
}

void ContextSteeringComponent::PhysicsTickComponent(float delta_time)
{
    ActorComponent::PhysicsTickComponent(delta_time);
    if (is_stopped_) return;

    DetectObstacles();

    if (rigid_body_)
    {
        rigid_body_->SetLinearVelocity(direction_ * speed_);
    }
}

void ContextSteeringComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    if (is_stopped_) return;
    
    direction_ = GetDirectionToMove();

    if (IsComplete())
    {
        is_stopped_ = true;
        rigid_body_->SetLinearVelocity(Math::Vector2::Zero());
    }
}

void ContextSteeringComponent::DetectObstacles()
{
    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();

    std::vector<Actor*> out_actors;
    Physics2D::OverlapCircleAll(position, 2.f, out_actors, static_cast<Type::uint16>(obstacle_layer_));
    obstacles_ = out_actors;
}

void ContextSteeringComponent::GetDangerSteering(float (&danger)[8])
{
    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();

    for (const auto& kActor : obstacles_)
    {
        IObstacle* obstacle = dynamic_cast<IObstacle*>(kActor);
        if (!obstacle) continue;

        Math::Vector2 direction = obstacle->GetCollider()->ClosestPoint(position) - position;

        float distance = direction.Magnitude();
        float weight = distance <= .6f ? 1.f : (2.f - distance) / 2.f;

        Math::Vector2 normalized = direction.Normalized();

        for (int i = 0; i < 8; ++i)
        {
            float result = Math::Vector2::Dot(normalized, directions_[i]);
            float score = result * weight;

            if (score > danger[i])
            {
                danger[i] = score;
            }
        }
    }
}

void ContextSteeringComponent::GetSeekSteering(float (&interest)[8])
{
    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();
    Math::Vector2 direction = destination_ - position;

    for (int i = 0; i < 8; ++i)
    {
        float result = Math::Vector2::Dot(direction.Normalized(), directions_[i]);
        if (result > 0.f)
        {
            float score = result;
            if (score > interest[i])
            {
                interest[i] = score;
            }
        }
    }
}

Math::Vector2 ContextSteeringComponent::GetDirectionToMove()
{
    float danger[8] = {0.f};
    float interest[8] = {0.f};

    GetDangerSteering(danger);
    GetSeekSteering(interest);

    DebugDrawHelper* debug_draw = DebugDrawHelper::Get();

    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();

    for (int i = 0; i < 8; ++i)
    {
        debug_draw->DrawRay(position, directions_[i] * danger[i] * 2.f, Math::Color::Red);
        debug_draw->DrawRay(position, directions_[i] * interest[i] * 2.f, Math::Color::Green);
    }

    for (int i = 0; i < 8; ++i)
    {
        interest[i] = Math::Clamp01(interest[i] - danger[i]);
    }

    Math::Vector2 result = Math::Vector2::Zero();
    for (int i = 0; i < 8; ++i)
    {
        result += directions_[i] * interest[i];
    }

    result.Normalize();
    
    debug_draw->DrawRay(GetOwner()->GetTransform()->GetPosition(), result * 2.f, Math::Color::Yellow);
    return result;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ContextSteeringComponent>("ContextSteeringComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

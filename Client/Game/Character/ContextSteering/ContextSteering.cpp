#include "pch.h"
#include "ContextSteering.h"

#include "DebugDrawHelper.h"
#include "IObstacle.h"
#include "Actor/Actor.h"
#include "Actor/Component/ColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"

std::vector<Math::Vector2> ContextSteering::directions_ = {
    Math::Vector2(0.f, 1.f).Normalized(),
    Math::Vector2(1.f, 1.f).Normalized(),
    Math::Vector2(1.f, 0.f).Normalized(),
    Math::Vector2(1.f, -1.f).Normalized(),
    Math::Vector2(0.f, -1.f).Normalized(),
    Math::Vector2(-1.f, -1.f).Normalized(),
    Math::Vector2(-1.f, 0.f).Normalized(),
    Math::Vector2(-1.f, 1.f).Normalized()
};

ContextSteering::ContextSteering(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    destination_(Math::Vector2::Zero()),
    obstacles_(),
    obstacle_layer_(ActorLayer::kBlock)
{
}

void ContextSteering::BeginPlay()
{
    ActorComponent::BeginPlay();

    ActorComponent* component = GetOwner()->GetComponent(RigidBody2DComponent::StaticClass());
    if (component) rigid_body_ = static_cast<RigidBody2DComponent*>(component);
}

void ContextSteering::PhysicsTickComponent(float delta_time)
{
    ActorComponent::PhysicsTickComponent(delta_time);

    DetectObstacles();
}

void ContextSteering::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    Math::Vector2 direction = GetDirectionToMove();
    DebugDrawHelper::Get()->DrawRay(GetOwner()->GetTransform()->GetPosition(), direction * 2.f, Math::Color::Yellow);

    if (rigid_body_) rigid_body_->SetLinearVelocity(direction);
}

void ContextSteering::DetectObstacles()
{
    Math::Vector2 position = GetOwner()->GetTransform()->GetPosition();

    std::vector<Actor*> out_actors;
    Physics2D::OverlapCircleAll(position, 2.f, out_actors, static_cast<Type::uint16>(obstacle_layer_));
    obstacles_ = out_actors;
}

void ContextSteering::GetDangerSteering(float (&danger)[8])
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

void ContextSteering::GetSeekSteering(float (&interest)[8])
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

Math::Vector2 ContextSteering::GetDirectionToMove()
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
    return result;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ContextSteering>("ContextSteering")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

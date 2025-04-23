#include "pch.h"
#include "Controller2D.h"

#include "Actor/Component/ColliderComponent.h"
#include "Math/Math.h"
#include <limits>

#include "DebugDrawHelper.h"
#include "Actor/Component/TransformComponent.h"
#include "Physics/Physics2D.h"

Controller2D::Controller2D(Actor* owner, const std::wstring& name) :
    ActorComponent(owner, name),
    kSkinWidth(.015f),
    horizontal_ray_count_(4),
    vertical_ray_count_(4),
    horizontal_ray_spacing_(0.f),
    vertical_ray_spacing_(0.f),
    collider_(nullptr),
    ray_cast_origins_(),
    collisions_()
{
}

void Controller2D::Move(Math::Vector2 velocity)
{
    UpdateRayCastOrigins();
    collisions_.Reset();

    if (!Math::IsEqual(velocity.x, 0.f)) HorizontalCollisions(velocity);
    if (!Math::IsEqual(velocity.y, 0.f)) VerticalCollisions(velocity);

    GetOwner()->GetTransform()->Translate(velocity);
}

void Controller2D::BeginPlay()
{
    ActorComponent::BeginPlay();

    collider_ = GetOwner()->GetComponent<ColliderComponent>(ColliderComponent::StaticClass());
    CalculateRaySpacing();
    
}

void Controller2D::UpdateRayCastOrigins()
{
    if (!collider_) return;
    
    Bounds bounds = collider_->GetBounds();
    bounds.Expand(kSkinWidth * -2.f);

    ray_cast_origins_.bottom_left = {bounds.min.x, bounds.min.y};
    ray_cast_origins_.bottom_right = {bounds.max.x, bounds.min.y};
    ray_cast_origins_.top_left = {bounds.min.x, bounds.max.y};
    ray_cast_origins_.top_right = {bounds.max.x, bounds.max.y};
    
}

void Controller2D::CalculateRaySpacing()
{
    if (!collider_) return;
    
    Bounds bounds = collider_->GetBounds();
    bounds.Expand(kSkinWidth * -2.f);

    horizontal_ray_count_ = Math::Clamp(horizontal_ray_count_, 2, std::numeric_limits<int>::max());
    vertical_ray_count_ = Math::Clamp(vertical_ray_count_, 2, std::numeric_limits<int>::max());

    horizontal_ray_spacing_ = bounds.size.x / (horizontal_ray_count_ - 1);
    vertical_ray_spacing_ = bounds.size.y / (vertical_ray_count_ - 1);
    
}

void Controller2D::HorizontalCollisions(Math::Vector2& velocity)
{
    float direction_x = Math::Sign(velocity.x);
    float ray_length = Math::Abs(velocity.x) + kSkinWidth;

    for (int i = 0; i < horizontal_ray_count_; ++i)
    {
        Math::Vector2 ray_origin = (direction_x == -1) ? ray_cast_origins_.bottom_left : ray_cast_origins_.bottom_right;
        ray_origin += Math::Vector2::Up() * (horizontal_ray_spacing_ * i);

        HitResult hit_result;
        bool is_hit = Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Right() * direction_x, ray_length, static_cast<Type::uint16>(ActorLayer::kDefault));

        DebugDrawHelper::Get()->DrawRay(ray_origin, Math::Vector2::Right() * direction_x * ray_length, is_hit ? Math::Color::Green : Math::Color::Red);
        
        if (is_hit)
        {
            velocity.x = (hit_result.distance - kSkinWidth) * direction_x;
            ray_length = hit_result.distance;

            collisions_.is_left = direction_x == -1;
            collisions_.is_right = direction_x == 1;
        }
    }
}

void Controller2D::VerticalCollisions(Math::Vector2& velocity)
{
    float direction_y = Math::Sign(velocity.y);
    float ray_length = Math::Abs(velocity.y) + kSkinWidth;

    for (int i = 0; i < vertical_ray_count_; ++i)
    {
        Math::Vector2 ray_origin = (direction_y == -1) ? ray_cast_origins_.bottom_left : ray_cast_origins_.top_left;
        ray_origin += Math::Vector2::Right() * (vertical_ray_spacing_ * i);

        HitResult hit_result;
        bool is_hit = Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Up() * direction_y, ray_length, static_cast<Type::uint16>(ActorLayer::kDefault));

        DebugDrawHelper::Get()->DrawRay(ray_origin, Math::Vector2::Up() * direction_y * ray_length, is_hit ? Math::Color::Green : Math::Color::Red);

        if (is_hit)
        {
            velocity.y = (hit_result.distance - kSkinWidth) * direction_y;
            ray_length = hit_result.distance;

            collisions_.is_below = direction_y == -1;
            collisions_.is_above = direction_y == 1;
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Controller2D>("Controller2D")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
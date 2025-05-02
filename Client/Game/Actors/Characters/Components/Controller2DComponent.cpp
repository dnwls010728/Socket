#include "pch.h"
#include "Controller2DComponent.h"

#include "Actor/Component/ColliderComponent.h"
#include "Math/Math.h"
#include <limits>

#include "DebugDrawHelper.h"
#include "Actor/Component/TransformComponent.h"
#include "Physics/Physics2D.h"

Controller2DComponent::Controller2DComponent(Actor* owner, const std::wstring& name) :
    ActorComponent(owner, name),
    kSkinWidth(.015f),
    horizontal_ray_count_(4),
    vertical_ray_count_(4),
    horizontal_ray_spacing_(0.f),
    vertical_ray_spacing_(0.f),
    max_climb_angle_(80.f),
    max_descend_angle_(80.f),
    collider_(nullptr),
    ray_cast_origins_(),
    collisions_()
{
}

void Controller2DComponent::Move(Math::Vector2 velocity)
{
    UpdateRayCastOrigins();
    collisions_.Reset();
    collisions_.velocity_old = velocity;

    if (velocity.y < 0.f) DescendSlope(velocity);
    
    if (!Math::IsEqual(velocity.x, 0.f)) HorizontalCollisions(velocity);
    if (!Math::IsEqual(velocity.y, 0.f)) VerticalCollisions(velocity);

    GetOwner()->GetTransform()->Translate(velocity);
}

void Controller2DComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    collider_ = GetOwner()->GetComponent<ColliderComponent>(ColliderComponent::StaticClass());
    CalculateRaySpacing();
    
}

void Controller2DComponent::UpdateRayCastOrigins()
{
    if (!collider_) return;
    
    Bounds bounds = collider_->GetBounds();
    bounds.Expand(kSkinWidth * -2.f);

    ray_cast_origins_.bottom_left = {bounds.min.x, bounds.min.y};
    ray_cast_origins_.bottom_right = {bounds.max.x, bounds.min.y};
    ray_cast_origins_.top_left = {bounds.min.x, bounds.max.y};
    ray_cast_origins_.top_right = {bounds.max.x, bounds.max.y};
    
}

void Controller2DComponent::CalculateRaySpacing()
{
    if (!collider_) return;
    
    Bounds bounds = collider_->GetBounds();
    bounds.Expand(kSkinWidth * -2.f);

    horizontal_ray_count_ = Math::Clamp(horizontal_ray_count_, 2, std::numeric_limits<int>::max());
    vertical_ray_count_ = Math::Clamp(vertical_ray_count_, 2, std::numeric_limits<int>::max());

    horizontal_ray_spacing_ = bounds.size.x / (horizontal_ray_count_ - 1);
    vertical_ray_spacing_ = bounds.size.y / (vertical_ray_count_ - 1);
    
}

void Controller2DComponent::HorizontalCollisions(Math::Vector2& velocity)
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
            float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
            if (i == 0 && slope_angle <= max_climb_angle_)
            {
                if (collisions_.is_descending_slope)
                {
                    collisions_.is_descending_slope = false;
                    velocity = collisions_.velocity_old;
                }
                
                float distance_to_slope_start = 0.f;
                if (!Math::IsEqual(slope_angle, collisions_.slope_angle_old))
                {
                    distance_to_slope_start = hit_result.distance - kSkinWidth;
                    velocity.x -= distance_to_slope_start * direction_x;
                }
                
                ClimbSlope(velocity, slope_angle);
                velocity.x += distance_to_slope_start * direction_x;
            }

            if (!collisions_.is_climbing_slope || slope_angle > max_climb_angle_)
            {
                velocity.x = (hit_result.distance - kSkinWidth) * direction_x;
                ray_length = hit_result.distance;

                if (collisions_.is_climbing_slope)
                {
                    velocity.y = std::tan(collisions_.slope_angle * Math::Deg2Rad()) * Math::Abs(velocity.x);
                }

                collisions_.is_left = direction_x == -1;
                collisions_.is_right = direction_x == 1;
            }
        }
    }
}

void Controller2DComponent::VerticalCollisions(Math::Vector2& velocity)
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

            if (collisions_.is_climbing_slope)
            {
                velocity.x = velocity.y / std::tan(collisions_.slope_angle * Math::Deg2Rad()) * Math::Sign(velocity.x);
            }

            collisions_.is_below = direction_y == -1;
            collisions_.is_above = direction_y == 1;
        }
    }
    
    if (collisions_.is_climbing_slope)
    {
        float direction_x = Math::Sign(velocity.x);
        ray_length = Math::Abs(velocity.x) + kSkinWidth;
            
        Math::Vector2 ray_origin = (direction_x == -1) ? ray_cast_origins_.bottom_left : ray_cast_origins_.bottom_right;
        ray_origin += Math::Vector2::Up() * velocity.y;

        HitResult hit_result;
        bool is_hit = Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Right() * direction_x, ray_length, static_cast<Type::uint16>(ActorLayer::kDefault));

        if (is_hit)
        {
            float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
            if (slope_angle != collisions_.slope_angle)
            {
                velocity.x = (hit_result.distance - kSkinWidth) * direction_x;
                collisions_.slope_angle = slope_angle;
            }
        }
    }
}

void Controller2DComponent::ClimbSlope(Math::Vector2& velocity, float slope_angle)
{
    float move_distance = Math::Abs(velocity.x);
    float climb_velocity_y = std::sin(slope_angle * Math::Deg2Rad()) * move_distance;

    if (velocity.y <= climb_velocity_y)
    {
        velocity.y = climb_velocity_y;
        velocity.x = std::cos(slope_angle * Math::Deg2Rad()) * move_distance * Math::Sign(velocity.x);
        collisions_.is_below = true;
        collisions_.is_climbing_slope = true;
        collisions_.slope_angle = slope_angle;
    }
}

void Controller2DComponent::DescendSlope(Math::Vector2& velocity)
{
    float direction_x = Math::Sign(velocity.x);
    Math::Vector2 ray_origin = (direction_x == -1) ? ray_cast_origins_.bottom_right : ray_cast_origins_.bottom_left;
    
    HitResult hit_result;
    bool is_hit = Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Down(), std::numeric_limits<float>::max(), static_cast<Type::uint16>(ActorLayer::kDefault));

    if (is_hit)
    {
        float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
        if (!Math::IsEqual(slope_angle, 0.f) && slope_angle <= max_descend_angle_)
        {
            if (Math::Sign(hit_result.normal.x) == direction_x)
            {
                if (hit_result.distance - kSkinWidth <= std::tan(slope_angle * Math::Deg2Rad()) * Math::Abs(velocity.x))
                {
                    float move_distance = Math::Abs(velocity.x);
                    float descend_velocity_y = std::sin(slope_angle * Math::Deg2Rad()) * move_distance;

                    velocity.x = std::cos(slope_angle * Math::Deg2Rad()) * move_distance * Math::Sign(velocity.x);
                    velocity.y -= descend_velocity_y;

                    collisions_.slope_angle = slope_angle;
                    collisions_.is_descending_slope = true;
                    collisions_.is_below = true;
                }
            }
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Controller2DComponent>("Controller2DComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
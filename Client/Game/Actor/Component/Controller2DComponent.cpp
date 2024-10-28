#include "pch.h"
#include "Controller2DComponent.h"

#include "DebugDrawHelper.h"
#include "Actor/Actor.h"
#include "Actor/Component/ColliderComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Bounds.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "rttr/registration.h"

Controller2DComponent::Controller2DComponent(Actor* owner, const std::wstring& kName) :
    RayCastController(owner, kName),
    slope_limit_(80.f),
    collisions_()
{
}

void Controller2DComponent::Move(Math::Vector2 move_amount)
{
    UpdateRaycastOrigins();
    collisions_.Reset();
    collisions_.velocity_old = move_amount;

    if (move_amount.y < 0.f) DescendSlope(move_amount);
    if (move_amount.x != 0.f) HorizontalCollisions(move_amount);
    if (move_amount.y != 0.f) VerticalCollisions(move_amount);
    
    GetOwner()->GetTransform()->Translate(move_amount);
}

void Controller2DComponent::HorizontalCollisions(Math::Vector2& move_amount)
{
    float dir_x = Math::Sign(move_amount.x);
    float ray_length = Math::Abs(move_amount.x) + skin_width_;

    for (int i = 0; i < horizontal_ray_count_; ++i)
    {
        Math::Vector2 ray_origin = dir_x == -1 ? raycast_origins_.bottom_left : raycast_origins_.bottom_right;
        ray_origin += Math::Vector2::Up() * (horizontal_ray_spacing_ * i);

        HitResult hit_result;
        if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Right() * dir_x, ray_length, static_cast<Type::uint16>(static_cast<Type::uint16>(ActorLayer::kGround))))
        {
            float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
            if (i == 0 && slope_angle <= slope_limit_)
            {
                if (collisions_.descending_slope)
                {
                    collisions_.descending_slope = false;
                    move_amount = collisions_.velocity_old;
                }
                
                float dist_to_slope_start = 0.f;
                if (slope_angle != collisions_.slope_angle_old)
                {
                    dist_to_slope_start = hit_result.distance - skin_width_;
                    move_amount.x -= dist_to_slope_start * dir_x;
                }
                
                ClimbSlope(move_amount, slope_angle, hit_result.normal);
                move_amount.x += dist_to_slope_start * dir_x;
            }

            if (!collisions_.climbing_slope || slope_angle > slope_limit_)
            {
                move_amount.x = (hit_result.distance - skin_width_) * dir_x;
                ray_length = hit_result.distance;

                if (collisions_.climbing_slope)
                {
                    move_amount.y = std::tan(collisions_.slope_angle * Math::Deg2Rad()) * Math::Abs(move_amount.x);
                }

                collisions_.left = dir_x == -1;
                collisions_.right = dir_x == 1;
            }
        }
    }
}

void Controller2DComponent::VerticalCollisions(Math::Vector2& move_amount)
{
    float dir_y = Math::Sign(move_amount.y);
    float ray_length = Math::Abs(move_amount.y) + skin_width_;
    
    for (int i = 0; i < vertical_ray_count_; ++i)
    {
        Math::Vector2 ray_origin = dir_y == -1 ? raycast_origins_.bottom_left : raycast_origins_.top_left;
        ray_origin += Math::Vector2::Right() * (vertical_ray_spacing_ * i + move_amount.x);

        HitResult hit_result;
        if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Up() * dir_y, ray_length, static_cast<Type::uint16>(ActorLayer::kGround)))
        {
            move_amount.y = (hit_result.distance - skin_width_) * dir_y;
            ray_length = hit_result.distance;

            if (collisions_.climbing_slope)
            {
                move_amount.x = move_amount.y / std::tan(collisions_.slope_angle * Math::Deg2Rad()) * Math::Sign(move_amount.x);
            }

            collisions_.below = dir_y == -1;
            collisions_.above = dir_y == 1;
        }
    }

    if (collisions_.climbing_slope)
    {
        float dir_x = Math::Sign(move_amount.x);
        ray_length = Math::Abs(move_amount.x) + skin_width_;
        Math::Vector2 ray_origin = (dir_x == -1 ? raycast_origins_.bottom_left : raycast_origins_.bottom_right) + Math::Vector2::Up() * move_amount.y;

        HitResult hit_result;
        if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Right() * dir_x, ray_length, static_cast<Type::uint16>(ActorLayer::kGround)))
        {
            float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
            if (slope_angle != collisions_.slope_angle)
            {
                move_amount.x = (hit_result.distance - skin_width_) * dir_x;
                collisions_.slope_angle = slope_angle;
                collisions_.slope_normal = hit_result.normal;
            }
        }
    }
}

void Controller2DComponent::ClimbSlope(Math::Vector2& move_amount, float slope_angle, const Math::Vector2& kSlopeNormal)
{
    float max_dist = Math::Abs(move_amount.x);
    float climb_vel_y = std::sin(slope_angle * Math::Deg2Rad()) * max_dist;
    
    if (move_amount.y <= climb_vel_y)
    {
        move_amount.y = climb_vel_y;
        move_amount.x = std::cos(slope_angle * Math::Deg2Rad()) * max_dist * Math::Sign(move_amount.x);
        collisions_.below = true;
        collisions_.climbing_slope = true;
        collisions_.slope_normal = kSlopeNormal;
    }
}

void Controller2DComponent::DescendSlope(Math::Vector2& move_amount)
{
    HitResult max_slope_hit_left;
    bool is_hit_left = Physics2D::RayCast(max_slope_hit_left, raycast_origins_.bottom_left, Math::Vector2::Down(), Math::Abs(move_amount.y) + skin_width_, static_cast<Type::uint16>(ActorLayer::kGround));

    HitResult max_slope_hit_right;
    bool is_hit_right = Physics2D::RayCast(max_slope_hit_right, raycast_origins_.bottom_right, Math::Vector2::Down(), Math::Abs(move_amount.y) + skin_width_, static_cast<Type::uint16>(ActorLayer::kGround));

    if (is_hit_left ^ is_hit_right)
    {
        SlideDownMaxSlope(max_slope_hit_left, move_amount);
        SlideDownMaxSlope(max_slope_hit_right, move_amount);
    }

    if (!collisions_.sliding_down_max_slope)
    {
        float dir_x = Math::Sign(move_amount.x);
        Math::Vector2 ray_origin = dir_x == -1 ? raycast_origins_.bottom_right : raycast_origins_.bottom_left;

        HitResult hit_result;
        if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Down(), FLT_MAX, static_cast<Type::uint16>(ActorLayer::kGround)))
        {
            float slope_angle = Math::Vector2::Angle(hit_result.normal, Math::Vector2::Up());
        
            if (slope_angle != 0.f && slope_angle <= slope_limit_)
            {
                if (Math::Sign(hit_result.normal.x) == dir_x)
                {
                    if (hit_result.distance - skin_width_ <= std::tan(slope_angle * Math::Deg2Rad()) * Math::Abs(move_amount.x))
                    {
                        float move_distance = Math::Abs(move_amount.x);
                        float descend_vel_y = std::sin(slope_angle * Math::Deg2Rad()) * move_distance;
                        move_amount.x = std::cos(slope_angle * Math::Deg2Rad()) * move_distance * Math::Sign(move_amount.x);
                        move_amount.y -= descend_vel_y;

                        collisions_.slope_angle = slope_angle;
                        collisions_.descending_slope = true;
                        collisions_.below = true;
                        collisions_.slope_normal = hit_result.normal;
                    }
                }
            }
        }
    }
}

void Controller2DComponent::SlideDownMaxSlope(const HitResult& kHit, Math::Vector2& move_amount)
{
    if (kHit.actor)
    {
        float slope_angle = Math::Vector2::Angle(kHit.normal, Math::Vector2::Up());
        if (slope_angle > slope_limit_)
        {
            move_amount.x = Math::Sign(kHit.normal.x) * (Math::Abs(move_amount.y) - kHit.distance) / std::tan(slope_angle * Math::Deg2Rad());

            collisions_.slope_angle = slope_angle;
            collisions_.sliding_down_max_slope = true;
            collisions_.slope_normal = kHit.normal;
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

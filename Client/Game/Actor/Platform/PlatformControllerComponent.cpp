#include "pch.h"
#include "PlatformControllerComponent.h"

#include <unordered_set>

#include "Actor/Actor.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "rttr/registration.h"
#include "rttr/detail/registration/registration_impl.h"

PlatformControllerComponent::PlatformControllerComponent(Actor* owner, const std::wstring& kName) :
    RayCastController(owner, kName),
    move_(Math::Vector2::Zero())
{
    move_ = {0.f, 1.f};
}

void PlatformControllerComponent::TickComponent(float delta_time)
{
    RayCastController::TickComponent(delta_time);
    
    UpdateRaycastOrigins();

    Math::Vector2 velocity = move_ * delta_time;
    MovePassengers(velocity);
    GetOwner()->GetTransform()->Translate(velocity);
}

void PlatformControllerComponent::MovePassengers(Math::Vector2 velocity)
{
    std::unordered_set<TransformComponent*> moved_passengers = {};
    
    float dir_x = Math::Sign(velocity.x);
    float dir_y = Math::Sign(velocity.y);

    if (velocity.y != 0.f)
    {
        float ray_length = Math::Abs(velocity.y) + skin_width_;

        for (int i = 0; i < vertical_ray_count_; ++i)
        {
            Math::Vector2 ray_origin = (dir_y == -1) ? raycast_origins_.bottom_left : raycast_origins_.top_left;
            ray_origin += Math::Vector2::Right() * (vertical_ray_spacing_ * i);

            HitResult hit_result;
            if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Up() * dir_y, ray_length, static_cast<Type::uint16>(ActorLayer::kPlayer)))
            {
                if (Actor* actor = hit_result.actor)
                {
                    if (TransformComponent* transform = actor->GetTransform(); !moved_passengers.contains(transform))
                    {
                        moved_passengers.insert(transform);
                        
                        float push_x = (dir_y == 1) ? velocity.x : 0.f;
                        float push_y = velocity.y - (hit_result.distance - skin_width_) * dir_y;

                        transform->Translate({push_x, push_y});
                    }
                }
            }
        }
    }

    if (velocity.x != 0.f)
    {
        float ray_length = Math::Abs(velocity.x) + skin_width_;

        for (int i = 0; i < horizontal_ray_count_; ++i)
        {
            Math::Vector2 ray_origin = (dir_x == -1) ? raycast_origins_.bottom_left : raycast_origins_.bottom_right;
            ray_origin += Math::Vector2::Up() * (horizontal_ray_spacing_ * i);

            HitResult hit_result;
            if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Right() * dir_x, ray_length, static_cast<Type::uint16>(ActorLayer::kPlayer)))
            {
                if (Actor* actor = hit_result.actor)
                {
                    if (TransformComponent* transform = actor->GetTransform(); !moved_passengers.contains(transform))
                    {
                        moved_passengers.insert(transform);
                        
                        float push_x = velocity.x - (hit_result.distance - skin_width_) * dir_x;
                        float push_y = 0.f;

                        transform->Translate({push_x, push_y});
                    }
                }
            }
        }

        if (dir_y == -1 || velocity.y == 0 || velocity.x != 0)
        {
            float ray_length = skin_width_ * 2.f;

            for (int i = 0; i < vertical_ray_count_; ++i)
            {
                Math::Vector2 ray_origin = raycast_origins_.top_left;
                ray_origin += Math::Vector2::Right() * (vertical_ray_spacing_ * i);

                HitResult hit_result;
                if (Physics2D::RayCast(hit_result, ray_origin, Math::Vector2::Up(), ray_length, static_cast<Type::uint16>(ActorLayer::kPlayer)))
                {
                    if (Actor* actor = hit_result.actor)
                    {
                        if (TransformComponent* transform = actor->GetTransform(); !moved_passengers.contains(transform))
                        {
                            moved_passengers.insert(transform);
                            
                            float push_x = velocity.x;
                            float push_y = velocity.y;

                            transform->Translate({push_x, push_y});
                        }
                    }
                }
            }
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlatformControllerComponent>("PlatformControllerComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

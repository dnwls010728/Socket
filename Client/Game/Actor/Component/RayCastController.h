#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Math/Vector2.h"

struct RaycastOrigins
{
    Math::Vector2 bottom_left;
    Math::Vector2 bottom_right;
    Math::Vector2 top_left;
    Math::Vector2 top_right;
};

class RayCastController : public ActorComponent
{
    SHADER_CLASS_HELPER(RayCastController)
    GENERATED_BODY(RayCastController, ActorComponent)
    
public:
    RayCastController(Actor* owner, const std::wstring& kName);
    virtual ~RayCastController() override = default;

protected:
    virtual void BeginPlay() override;
    
    void UpdateRaycastOrigins();
    void CalculateRaySpecing();
    
    class ColliderComponent* collider_;
    
    float skin_width_;
    float horizontal_ray_spacing_;
    float vertical_ray_spacing_;
    
    int horizontal_ray_count_;
    int vertical_ray_count_;
    
    RaycastOrigins raycast_origins_;
    
};

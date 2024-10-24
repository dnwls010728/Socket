#pragma once
#include "RayCastController.h"
#include "Math/Vector2.h"

struct HitResult;

struct CollisionInfo
{
    bool above;
    bool below;
    bool left;
    bool right;
    bool climbing_slope;
    bool descending_slope;
    bool sliding_down_max_slope;

    float slope_angle;
    float slope_angle_old;

    Math::Vector2 velocity_old;
    Math::Vector2 slope_normal;

    void Reset()
    {
        above = false;
        below = false;
        left = false;
        right = false;
        climbing_slope = false;
        descending_slope = false;
        sliding_down_max_slope = false;

        slope_angle_old = slope_angle;
        slope_normal = Math::Vector2::Zero();
        slope_angle = 0;
    }
};

class Controller2DComponent : public RayCastController
{
    SHADER_CLASS_HELPER(Controller2DComponent)
    GENERATED_BODY(Controller2DComponent, RayCastController)
    
public:
    Controller2DComponent(Actor* owner, const std::wstring& kName);
    virtual ~Controller2DComponent() override = default;
    
    void Move(Math::Vector2 move_amount);

    FORCEINLINE const CollisionInfo& GetCollisions() const { return collisions_; }

private:
    void HorizontalCollisions(Math::Vector2& move_amount);
    void VerticalCollisions(Math::Vector2& move_amount);
    void ClimbSlope(Math::Vector2& move_amount, float slope_angle, const Math::Vector2& kSlopeNormal);
    void DescendSlope(Math::Vector2& move_amount);
    void SlideDownMaxSlope(const HitResult& kHit, Math::Vector2& move_amount);

    float slope_limit_;

    CollisionInfo collisions_;
    
};

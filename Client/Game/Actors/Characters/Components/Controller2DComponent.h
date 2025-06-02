#pragma once
#include "Actor/Component/ActorComponent.h"

class Controller2DComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(Controller2DComponent)
    GENERATED_BODY(Controller2DComponent, ActorComponent)

public:
    struct RayCastOrigins
    {
        Math::Vector2 top_left;
        Math::Vector2 top_right;
        Math::Vector2 bottom_left;
        Math::Vector2 bottom_right;
    };

    struct CollisionInfo
    {
        bool is_above;
        bool is_below;
        bool is_left;
        bool is_right;
        bool is_climbing_slope;
        bool is_descending_slope;
        bool is_falling;

        float slope_angle;
        float slope_angle_old;

        Math::Vector2 velocity_old;

        void Reset()
        {
            is_above = false;
            is_below = false;
            is_left = false;
            is_right = false;
            is_climbing_slope = false;
            is_descending_slope = false;

            slope_angle_old = slope_angle;
            slope_angle = 0.f;
        }
    };
    
public:
    Controller2DComponent(Actor* owner, const std::wstring& name);
    virtual ~Controller2DComponent() override = default;

    void Move(const Math::Vector2& move_amount);
    void Move(Math::Vector2 move_amount, const Math::Vector2& input);

    FORCEINLINE const CollisionInfo& GetCollisions() const { return collisions_; }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EndPlayReason type) override;

private:
    void UpdateRayCastOrigins();
    void CalculateRaySpacing();
    void HorizontalCollisions(Math::Vector2& move_amount);
    void VerticalCollisions(Math::Vector2& move_amount);
    void ClimbSlope(Math::Vector2& move_amount, float slope_angle);
    void DescendSlope(Math::Vector2& move_amount);
    
    const float kSkinWidth;

    int32_t horizontal_ray_count_;
    int32_t vertical_ray_count_;

    float horizontal_ray_spacing_;
    float vertical_ray_spacing_;
    float max_climb_angle_;
    float max_descend_angle_;

#pragma region 컴포넌트
    std::shared_ptr<class ColliderComponent> collider_;
#pragma endregion

    RayCastOrigins ray_cast_origins_;
    CollisionInfo collisions_;

    Math::Vector2 input_;

    TimerHandle timer_handle_;
    
};

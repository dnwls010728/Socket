#pragma once
#include "Actor/Component/ActorComponent.h"

class Controller2D : public ActorComponent
{
    SHADER_CLASS_HELPER(Controller2D)
    GENERATED_BODY(Controller2D, ActorComponent)

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

        void Reset()
        {
            is_above = false;
            is_below = false;
            is_left = false;
            is_right = false;
        }
    };
    
public:
    Controller2D(Actor* owner, const std::wstring& name);
    virtual ~Controller2D() override = default;

    void Move(Math::Vector2 velocity);

    FORCEINLINE const CollisionInfo& GetCollisions() const { return collisions_; }

protected:
    virtual void BeginPlay() override;

private:
    void UpdateRayCastOrigins();
    void CalculateRaySpacing();
    void HorizontalCollisions(Math::Vector2& velocity);
    void VerticalCollisions(Math::Vector2& velocity);
    
    const float kSkinWidth;

    int horizontal_ray_count_;
    int vertical_ray_count_;

    float horizontal_ray_spacing_;
    float vertical_ray_spacing_;

#pragma region 컴포넌트
    std::shared_ptr<class ColliderComponent> collider_;
#pragma endregion

    RayCastOrigins ray_cast_origins_;
    CollisionInfo collisions_;
    
};

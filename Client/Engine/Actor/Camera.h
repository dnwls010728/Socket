#pragma once
#include "Actor.h"
#include "Math/Bounds.h"

struct FocusArea
{
    FocusArea() = default;
    
    void Setup(const Bounds& kBounds, const Math::Vector2& kSize)
    {
        left = kBounds.center.x - kSize.x / 2.f;
        right = kBounds.center.x + kSize.x / 2.f;
        top = kBounds.min.y + kSize.y;
        bottom = kBounds.min.y;

        center = {(left + right) / 2.f, (top + bottom) / 2.f};
    }

    void Update(const Bounds& bounds)
    {
        float shift_x = 0.f;
        if (bounds.min.x < left)
        {
            shift_x = bounds.min.x - left;
        }
        else if (bounds.max.x > right)
        {
            shift_x = bounds.max.x - right;
        }
        left += shift_x;
        right += shift_x;

        float shift_y = 0.f;
        if (bounds.min.y < bottom)
        {
            shift_y = bounds.min.y - bottom;
        }
        else if (bounds.max.y > top)
        {
            shift_y = bounds.max.y - top;
        }
        top += shift_y;
        bottom += shift_y;

        center = {(left + right) / 2.f, (top + bottom) / 2.f};
        velocity = {shift_x, shift_y};
    }
    
    Math::Vector2 center;
    Math::Vector2 velocity;

    float left;
    float right;
    float top;
    float bottom;
};

class Camera : public Actor
{
    SHADER_CLASS_HELPER(Camera)
    GENERATED_BODY(Camera, Actor)
    
public:
    Camera(const std::wstring& kName);
    virtual ~Camera() override = default;

    virtual void PreInitializeComponents() override;
    virtual void PhysicsTick(float delta_time) override;

    static Camera* Get();

    void SetSize(float size);
    void SetNearZ(float near_z);
    void SetFarZ(float far_z);
    void SetTarget(Actor* target);

    float GetAspect() const;

    Bounds GetBounds() const;

    FORCEINLINE float GetSize() const { return size_; }
    FORCEINLINE float GetNearZ() const { return near_z_; }
    FORCEINLINE float GetFarZ() const { return far_z_; }

private:
    friend class Level;
    
    void UpdateProjectionMatrix() const;
    
    static std::weak_ptr<Camera> camera_;
    
    float size_;
    float near_z_;
    float far_z_;

    Actor* target_;

    class ColliderComponent* target_collider_;

    FocusArea focus_area_;
    
    Math::Vector2 focus_area_size_;
    
    float vertical_offset_;

    Type::uint32 half_width_;
    Type::uint32 half_height_;
    Type::uint32 limit_half_width_;
    Type::uint32 limit_half_height_;
    
};

#pragma once
#include "Math/Bounds.h"

class Actor;

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

enum class TickType
{
    kPhysicsTick,
    kTick,
    kPostTick,
};

class CameraManager : public Singleton<CameraManager>
{
public:
    CameraManager();
    virtual ~CameraManager() override = default;

    float GetAspect();

    void SetPosition(const Math::Vector2& kPosition);
    void SetAngle(float angle);
    void SetSize(float size);
    void SetNearZ(float near_z);
    void SetFarZ(float far_z);
    void SetTarget(const std::shared_ptr<Actor>& kActor);

    const Bounds& GetBounds();

    FORCEINLINE void SetTickType(TickType kTickType) { tick_type_ = kTickType; }
    FORCEINLINE TickType GetTickType() const { return tick_type_; }

private:
    friend class World;
    
    void Init();
    void PhysicsTick(float delta_time);
    void Tick(float delta_time);
    void PostTick(float delta_time);
    void UpdateProjectionMatrix();
    void UpdateViewMatrix();
    void MoveToTarget(float delta_time);

    float size_;
    float near_z_;
    float far_z_;
    float angle_;
    float vertical_offset_;
    float limit_half_width_;
    float limit_half_height_;

    FocusArea focus_area_;

    std::weak_ptr<Actor> target_weak_ptr;
    std::weak_ptr<class ColliderComponent> collider_weak_ptr_;

    Math::Vector2 position_;
    Math::Vector2 focus_area_size_;

    TickType tick_type_;
    
};

#pragma once

#include "ActorComponent.h"
#include "Engine/Math/Vector2.h"

class TransformComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(TransformComponent)
    GENERATED_BODY(TransformComponent, ActorComponent)
    
public:
    TransformComponent(Actor* owner, const std::wstring& kName);
    virtual ~TransformComponent() override = default;

    void SetPosition(const Math::Vector2& position);
    void SetAngle(float angle);
    void SetScale(const Math::Vector2& scale);
    void Translate(const Math::Vector2& translation);

    Math::Vector2 GetRightVector() const;
    Math::Vector2 GetUpVector() const;

    FORCEINLINE Math::Vector2 GetPosition() const { return position_; }
    FORCEINLINE Math::Vector2 GetScale() const { return scale_; }

    FORCEINLINE float GetAngle() const { return angle_; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    friend class Actor;
    friend class ColliderComponent;
    friend class RigidBody2DComponent;

    void UpdateBody();

    Math::Vector2 position_;
    Math::Vector2 scale_;

    float angle_;
    
};

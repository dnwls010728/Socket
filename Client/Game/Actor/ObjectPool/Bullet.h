#pragma once
#include "PooledObject.h"

class Bullet : public PooledObject
{
    SHADER_CLASS_HELPER(Bullet)
    GENERATED_BODY(Bullet, PooledObject)
    
public:
    Bullet(const std::wstring& kName);
    virtual ~Bullet() override = default;

    FORCEINLINE RigidBody2DComponent* GetRigidBody() const { return rigid_body_; }

protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void OnEnable() override;

private:
    class CircleColliderComponent* circle_collider_;
    RigidBody2DComponent* rigid_body_;
    class SpriteRendererComponent* renderer_;

    class Sprite* sprite_;
    
};

#pragma once
#include "PooledObject.h"

class Bullet : public PooledObject
{
    SHADER_CLASS_HELPER(Bullet)
    GENERATED_BODY(Bullet, PooledObject)
    
public:
    Bullet(const std::wstring& kName);
    virtual ~Bullet() override = default;

    virtual void Deactivate() override;

    FORCEINLINE RigidBody2DComponent* GetRigidBody() const { return rigid_body_; }

protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void OnEnable() override;

private:
    class CircleColliderComponent* circle_collider_;
    RigidBody2DComponent* rigid_body_;
    class SpriteRendererComponent* renderer_;
    class ObjectPoolComponent* hit_effect_pool_;

    class Sprite* sprite_;

    TimerHandle life_timer_;
    
};

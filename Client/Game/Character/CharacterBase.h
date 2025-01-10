#pragma once
#include "Actor/Actor.h"

class AnimatorComponent;
class SpriteRendererComponent;

class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    FORCEINLINE SpriteRendererComponent* GetRenderer() const { return renderer_; }
    FORCEINLINE CircleColliderComponent* GetCollider() const { return collider_; }
    FORCEINLINE RigidBody2DComponent* GetRigidBody() const { return rigid_body_; }
    FORCEINLINE AnimatorComponent* GetAnimator() const { return animator_; }

    FORCEINLINE bool IsDead() const { return is_dead_; }

protected:
    virtual void OnHit();
    virtual void OnDeath();
    
    SpriteRendererComponent* renderer_;
    CircleColliderComponent* collider_;
    RigidBody2DComponent* rigid_body_;
    AnimatorComponent* animator_;

    bool is_dead_;
    
};

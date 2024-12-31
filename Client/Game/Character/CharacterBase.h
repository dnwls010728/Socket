#pragma once
#include "Actor/NetworkActor.h"

class StateMachine;
class AnimatorComponent;
class SpriteRendererComponent;

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    FORCEINLINE SpriteRendererComponent* GetRenderer() const { return renderer_; }
    FORCEINLINE CapsuleColliderComponent* GetCollider() const { return collider_; }
    FORCEINLINE RigidBody2DComponent* GetRigidBody() const { return rigid_body_; }
    FORCEINLINE AnimatorComponent* GetAnimator() const { return animator_; }

    FORCEINLINE bool IsDead() const { return is_dead_; }

protected:
    /**
     * 현재 사용하지 않음
     */
    void AdjustPosition() const;
    
    virtual void OnHit();
    virtual void OnDeath();
    
    SpriteRendererComponent* renderer_;
    CapsuleColliderComponent* collider_;
    RigidBody2DComponent* rigid_body_;
    AnimatorComponent* animator_;
    StateMachine* state_machine_;

    bool is_dead_;
    
};

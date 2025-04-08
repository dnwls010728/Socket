#pragma once
#include "Actor/Actor.h"

class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    FORCEINLINE std::shared_ptr<class CircleColliderComponent> GetCollider() const { return collider_; }
    FORCEINLINE std::shared_ptr<class RigidBody2DComponent> GetRigidBody() const { return rigid_body_; }
    FORCEINLINE std::shared_ptr<class SpriteRendererComponent> GetRenderer() const { return renderer_; }
    FORCEINLINE std::shared_ptr<class StateMachineComponent> GetStateMachine() const { return state_machine_; }

protected:
    virtual void Tick(float delta_time) override;
    
#pragma region 컴포넌트
    std::shared_ptr<class CircleColliderComponent> collider_;
    std::shared_ptr<class RigidBody2DComponent> rigid_body_;
    std::shared_ptr<class SpriteRendererComponent> renderer_;
    std::shared_ptr<class StateMachineComponent> state_machine_;
#pragma endregion
    
};

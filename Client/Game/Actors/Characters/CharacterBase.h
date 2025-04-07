#pragma once
#include "Actor/Actor.h"

class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

protected:
    virtual void Tick(float delta_time) override;
    
#pragma region 컴포넌트
    std::shared_ptr<class CircleColliderComponent> collider_;
    std::shared_ptr<class RigidBody2DComponent> rigid_body_;
    std::shared_ptr<class SpriteRendererComponent> renderer_;
#pragma endregion
    
};

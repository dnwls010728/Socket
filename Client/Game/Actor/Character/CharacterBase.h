#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    bool IsGrounded() const;

protected:
    virtual void Tick(float delta_time) override;
    
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class Controller2DComponent* controller_;

    float gravity_;

    Math::Vector2 velocity_;
    
};

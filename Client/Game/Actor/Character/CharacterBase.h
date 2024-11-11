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
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class RigidBody2DComponent* rigid_body_;
    
};

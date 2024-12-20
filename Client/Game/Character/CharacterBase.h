#pragma once
#include "Actor/NetworkActor.h"

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    virtual void OnDamaged(float damage);

protected:
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class RigidBody2DComponent* rigid_body_;
    
};

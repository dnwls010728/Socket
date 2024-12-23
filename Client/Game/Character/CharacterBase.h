#pragma once
#include "Actor/NetworkActor.h"

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    static float ApplyDamage(CharacterBase* damaged_character, float base_damage, Actor* event_instigator, Actor* damage_causer);

protected:
    virtual float TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer);
    
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class RigidBody2DComponent* rigid_body_;
    
};

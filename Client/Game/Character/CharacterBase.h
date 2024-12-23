#pragma once
#include "Actor/NetworkActor.h"

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    float ApplyDamage(CharacterBase* damaged_character, float base_damage, CharacterBase* event_instigator, CharacterBase* damage_causer);
    
    virtual float TakeDamage(float damage_amount, CharacterBase* event_instigator, CharacterBase* damage_causer);

protected:
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class RigidBody2DComponent* rigid_body_;
    
};

#pragma once
#include "Actor/NetworkActor.h"

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    /**
     * 캐릭터에게 데미지를 적용합니다.
     * @param damaged_character 피해를 입은 캐릭터
     * @param base_damage 적용할 기본 데미지
     * @param event_instigator 피해를 유발한 책임이 있는 액터
     * @param damage_causer 실제로 피해를 입힌 액터
     * @return 적용된 데미지
     */
    static float ApplyDamage(CharacterBase* damaged_character, float base_damage, Actor* event_instigator, Actor* damage_causer);

    FORCEINLINE bool IsDead() const { return is_dead_; }

protected:
    virtual float TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer);

    virtual void OnHit();
    virtual void OnDeath();
    
    class SpriteRendererComponent* renderer_;
    class CapsuleColliderComponent* collider_;
    class RigidBody2DComponent* rigid_body_;

    bool is_dead_;
    
};

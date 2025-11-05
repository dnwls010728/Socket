#pragma once

#include <string>

#include "Skill/AttackEffect.h"

class DamageEffect : public  AttackEffect
{
public:
    DamageEffect();

    void Apply(const AttackContext& ctx) override;

    inline int32_t GetBaseDamage() const { return base_damage_; }
    inline void SetBaseDamage(int32_t value) { base_damage_ = value; }
    
    inline float GetDamageMultiplier() const { return damage_multiplier_; }
    inline void SetDamageMultiplier(float value) { damage_multiplier_ = value; }
    
    inline void SetAttackCount(int32_t value) { attack_count_ = value; }
    inline int32_t GetAttackCount() const { return attack_count_; }

private:
    int32_t base_damage_;
    float damage_multiplier_;
    bool can_critical_;
    int32_t attack_count_;
};

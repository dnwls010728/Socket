#pragma once

#include "Skill/AttackEffect.h"

class DamageEffect : public  AttackEffect
{
public:
    DamageEffect();

    void Apply(const AttackContext& ctx) override;

    inline int GetBaseDamage() const { return base_damage_; }
    inline void SetBaseDamage(int value) { base_damage_ = value; }
    
    inline float GetDamageMultiplier() const { return damage_multiplier_; }
    inline void SetDamageMultiplier(float value) { damage_multiplier_ = value; }
    
    inline bool CanCritical() const { return can_critical_; }
    inline void SetCanCritical(bool value) { can_critical_ = value; }

private:
    int base_damage_;
    float damage_multiplier_;
    bool can_critical_;
};

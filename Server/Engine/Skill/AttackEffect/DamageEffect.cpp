#include "pch.h"
#include "DamageEffect.h"
#include "Map/MapObjects/Mob/Mob.h"

DamageEffect::DamageEffect() :
    damage_multiplier_(100.0f),
    base_damage_(0),
    can_critical_(false),
    attack_count_(1)
{
}

void DamageEffect::Apply(const AttackContext& ctx)
{
    int32_t total_damage = base_damage_ + static_cast<int32_t>(damage_multiplier_ * ctx.attacker->GetAtk() );
    ctx.target->TakeMultiDamage(ctx.attacker->GetObjectID() ,std::vector<int32_t>(attack_count_, total_damage));
}
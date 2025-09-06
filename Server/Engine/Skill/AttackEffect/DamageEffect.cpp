#include "pch.h"
#include "DamageEffect.h"
#include "Map/MapObjects/Mob/Mob.h"

DamageEffect::DamageEffect() :
    damage_multiplier_(100.0f),
    base_damage_(0),
    can_critical_(false)
{
}

void DamageEffect::Apply(const AttackContext& ctx)
{
    int total_damage = base_damage_ + static_cast<int>(damage_multiplier_ * ctx.attacker->GetLv() * 50); //ctx.attacker->GetDamage();
    ctx.target->TakeDamage(ctx.attacker->GetObjectID() ,total_damage);
}
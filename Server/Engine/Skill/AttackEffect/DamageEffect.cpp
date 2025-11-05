#include "pch.h"
#include "DamageEffect.h"

#include <vector>

#include "Math/Bounds.h"
#include "Math/Math.h"

DamageEffect::DamageEffect() :
    damage_multiplier_(100.0f),
    base_damage_(0),
    can_critical_(false),
    attack_count_(1),
    hit_effect_pack_(),
    hit_effect_animation_()
{
}

void DamageEffect::Apply(const AttackContext& ctx)
{
    if (ctx.attacker == nullptr || ctx.target == nullptr) return;

    int32_t total_damage = base_damage_ + static_cast<int32_t>(damage_multiplier_ * ctx.attacker->GetAtk());

    Bounds target_bounds = ctx.target->GetBounds();
    Bounds hit_bounds = ctx.has_hitbox ? Bounds::Intersect(ctx.hitbox, target_bounds) : target_bounds;
    
    
    const Bounds& spawn_bounds = (hit_bounds.size.x > 0.f && hit_bounds.size.y > 0.f) ? hit_bounds : target_bounds;
    float x = Math::RandRange(spawn_bounds.min.x, spawn_bounds.max.x);
    float y = Math::RandRange(spawn_bounds.min.y, spawn_bounds.max.y);
    Math::Vector2 position = {x, y};
    
    std::vector<DamageHitInfo> damages;
    damages.reserve(static_cast<size_t>(attack_count_));
    for (int32_t i = 0; i < attack_count_; ++i)
    {
        DamageHitInfo info;
        info.damage_amount = total_damage;
        info.position = position;
        info.effect_pack = hit_effect_pack_;
        info.effect_animation = hit_effect_animation_;
        damages.push_back(info);
    }

    if (damages.empty()) return;

    ctx.target->TakeMultiDamage(ctx.attacker->GetObjectID(), damages);
}

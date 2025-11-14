#include "pch.h"
#include "DamageEffect.h"

#include <vector>

#include "Math/Bounds.h"
#include "Math/Math.h"

DamageEffect::DamageEffect() :
    damage_multiplier_(100.0f),
    base_damage_(0),
    can_critical_(false),
    attack_count_(1)
{
}

void DamageEffect::Apply(const AttackContext& ctx)
{
    if (ctx.attacker == nullptr || ctx.target == nullptr) return;

    int32_t total_def = ctx.target->GetHitDef() - static_cast<int32_t>(ctx.target->GetHitDef() * (1 + ctx.attacker->GetDef() * .01f));
    int32_t total_damage = static_cast<int32_t>(Math::Pow(ctx.attacker->GetAtk(), 2) / (ctx.attacker->GetAtk() + Math::Max(0, total_def)) * damage_multiplier_) * ctx.source_level;

    Bounds target_bounds = ctx.target->GetHitBounds();
    Bounds hit_bounds = Bounds::Intersect(ctx.hitbox, target_bounds);
    
    const Bounds& spawn_bounds = (hit_bounds.size.x > 0.f && hit_bounds.size.y > 0.f) ? hit_bounds : target_bounds;

    float direction = ctx.attacker->GetPosition().x - ctx.target->GetHitPosition().x;
    
    std::vector<DamageHitInfo> damages;
    damages.reserve(static_cast<size_t>(attack_count_));
    for (int32_t i = 0; i < attack_count_; ++i)
    {
        float x = Math::RandRange(spawn_bounds.min.x, spawn_bounds.max.x);
        float y = Math::RandRange(spawn_bounds.min.y, spawn_bounds.max.y);
        Math::Vector2 position = {x, y};
        
        DamageHitInfo info;
        info.damage_amount = total_damage;
        info.position = position;
        info.attacker_direction = Math::Sign(direction);
        info.source_type = ctx.source_type;
        info.source_id = ctx.source_id;
        damages.push_back(info);
    }

    if (damages.empty()) return;

    ctx.target->TakeMultiDamage(ctx.attacker->GetObjectID(), damages);
}

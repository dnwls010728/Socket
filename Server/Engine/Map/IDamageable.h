#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Math/Bounds.h"
#include "Math/Vector2.h"

struct DamageHitInfo
{
    int32_t damage_amount;
    Math::Vector2 position;
    std::wstring effect_pack;
    std::wstring effect_animation;
};

class IDamageable
{
public:
    virtual ~IDamageable() = default;

    virtual Bounds GetDamageBounds() const = 0;

    virtual void TakeDamage(uint32_t attacker, const DamageHitInfo& damage) = 0;
    virtual void TakeMultiDamage(uint32_t attacker, const std::vector<DamageHitInfo>& damages) = 0;
};

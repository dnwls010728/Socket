#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Math/Bounds.h"
#include "Math/Vector2.h"
#include <CommonObject.h>

struct DamageHitInfo
{
    int32_t damage_amount;
    Math::Vector2 position;
    float attacker_direction;
    DamageSourceType source_type;
    uint32_t source_id;
};

class IDamageable
{
public:
    virtual ~IDamageable() = default;

    virtual Bounds GetHitBounds() const = 0;
    virtual Math::Vector2 GetHitPosition() const = 0;
    virtual int32_t GetHitDef() const = 0;

    virtual void TakeDamage(uint32_t attacker, const DamageHitInfo& damage) = 0;
    virtual void TakeMultiDamage(uint32_t attacker, const std::vector<DamageHitInfo>& damages) = 0;
};

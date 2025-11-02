#pragma once
#include <memory>

#include "Map/IDamageable.h"
#include "Map/PlayerCharacter.h"
#include "Math/Bounds.h"
class AttackEffect
{
public:
    struct AttackContext
    {
        PlayerCharacter* attacker;
        IDamageable* target;
        Bounds hitbox;
        bool has_hitbox{false};
    };

    AttackEffect() = default;
    virtual void Apply(const AttackContext& ctx) = 0;
};
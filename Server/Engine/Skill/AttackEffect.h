#pragma once
#include <memory>

#include "Map/IDamageable.h"
#include "Map/PlayerCharacter.h"
class AttackEffect
{
public:
    struct AttackContext
    {
        PlayerCharacter* attacker;
        IDamageable* target;
    };

    AttackEffect() = default;
    virtual void Apply(const AttackContext& ctx) = 0;
};
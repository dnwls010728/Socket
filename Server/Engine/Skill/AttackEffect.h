#pragma once
#include <memory>

#include "Map/PlayerCharacter.h"

class Mob;
class Player;
class AttackEffect
{
public:
    struct AttackContext
    {
        PlayerCharacter* attacker;
        Mob* target;
    };

    AttackEffect() = default;
    virtual void Apply(const AttackContext& ctx) = 0;
};
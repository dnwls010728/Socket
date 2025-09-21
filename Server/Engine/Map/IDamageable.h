#pragma once

#include <cstdint>
#include <vector>

class IDamageable
{
public:
    virtual ~IDamageable() = default;

    virtual void TakeDamage(uint32_t attacker, int32_t damage) = 0;
    virtual void TakeMultiDamage(uint32_t attacker, const std::vector<int32_t>& damages) = 0;
};

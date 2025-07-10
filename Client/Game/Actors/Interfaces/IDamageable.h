#pragma once

class IDamageable
{
public:
    virtual ~IDamageable() = default;

    virtual void TakeDamage(uint32_t updated_hp, uint32_t damage_amount, float server_time) = 0;
    
};

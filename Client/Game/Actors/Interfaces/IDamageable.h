#pragma once

class IDamageable
{
public:
    virtual ~IDamageable() = default;

    virtual void TakeDamage(int32_t damage_amount, float server_time) = 0;
    
};

#pragma once

class Tickable
{
public:
    Tickable();
    virtual ~Tickable() = default;

    virtual void PhysicsTick(float delta_time);
    virtual void Tick(float delta_time);
    virtual void PostTick(float delta_time);
    
};

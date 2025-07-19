#pragma once

class IPoolable
{
public:
    IPoolable() = default;
    virtual ~IPoolable() = default;

    virtual void OnActivate() = 0;
    virtual void OnDeactivate() = 0;
    
};

#pragma once
#include <memory>

class StatEffect : std::enable_shared_from_this<StatEffect>
{
public:
    StatEffect();
    virtual ~StatEffect() = default;
    
};

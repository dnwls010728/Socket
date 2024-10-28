#pragma once
#include "Actor/Actor.h"

class Network : public Actor
{
    SHADER_CLASS_HELPER(Network)
    GENERATED_BODY(Network, Actor)
    
public:
    Network(const std::wstring& kName);
    virtual ~Network() override = default;

protected:
    virtual void Tick(float deltaTime) override;
    
};

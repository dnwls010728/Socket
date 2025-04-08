#pragma once
#include "Actor/Actor.h"

class Dummy : public Actor
{
    SHADER_CLASS_HELPER(Dummy)
    GENERATED_BODY(Dummy, Actor)
    
public:
    Dummy(const std::wstring& kName);
    virtual ~Dummy() override = default;

protected:
    std::shared_ptr<class CircleColliderComponent> collider_;
    
};

#pragma once
#include "Actor/Actor.h"

class NetworkActor : public Actor
{
    SHADER_CLASS_HELPER(NetworkActor)
    GENERATED_BODY(NetworkActor, Actor)
    
public:
    NetworkActor(const std::wstring& kName);
    virtual ~NetworkActor() override = default;

    FORCEINLINE Type::uint32 GetID() const { return id_; }

private:
    friend class NetworkSubsystem;
    
    Type::uint32 id_;
    
};

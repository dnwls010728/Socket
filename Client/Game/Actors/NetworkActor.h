#pragma once
#include "Actor/Actor.h"

class NetworkActor : public Actor
{
    SHADER_CLASS_HELPER(NetworkActor)
    GENERATED_BODY(NetworkActor, Actor)
    
public:
    NetworkActor(const std::wstring& kName);
    virtual ~NetworkActor() override = default;

    FORCEINLINE void SetUniqueID(Type::uint32 id) { unique_id_ = id; }
    FORCEINLINE Type::uint32 GetUniqueID() const { return unique_id_; }

private:
    Type::uint32 unique_id_;
    
};

#pragma once
#include "Actor/Actor.h"

namespace Net
{
    struct IPacket;
}

class NetworkActor : public Actor
{
    SHADER_CLASS_HELPER(NetworkActor)
    GENERATED_BODY(NetworkActor, Actor)
    
public:
    NetworkActor(const std::wstring& kName);
    virtual ~NetworkActor() override = default;

    void SendPacket(Net::IPacket& packet);

    FORCEINLINE void SetUniqueID(Type::uint32 id) { unique_id_ = id; }
    FORCEINLINE Type::uint32 GetUniqueID() const { return unique_id_; }

    FORCEINLINE bool IsOwner() const { return is_owner_; }
    FORCEINLINE void SetOwner(bool is_owner) { is_owner_ = is_owner; }

private:
    Type::uint32 unique_id_;

    bool is_owner_;
    
};

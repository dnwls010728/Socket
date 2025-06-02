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
    
    virtual void ReceivePacket(Net::IPacket* packet);

    FORCEINLINE void SetObjectID(uint32_t object_id) { object_id_ = object_id; }
    FORCEINLINE uint32_t GetObjectID() const { return object_id_; }

    FORCEINLINE bool IsMine() const { return is_mine_; }
    FORCEINLINE void SetMine(bool is_mine) { is_mine_ = is_mine; }

private:
    uint32_t object_id_;

    bool is_mine_;
    
};

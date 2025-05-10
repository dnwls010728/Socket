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

    FORCEINLINE void SetUniqueID(uint32_t id) { unique_id_ = id; }
    FORCEINLINE uint32_t GetUniqueID() const { return unique_id_; }

    FORCEINLINE bool IsMine() const { return is_mine_; }
    FORCEINLINE void SetMine(bool is_mine) { is_mine_ = is_mine; }

private:
    uint32_t unique_id_;

    bool is_mine_;
    
};

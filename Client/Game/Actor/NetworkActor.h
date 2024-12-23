#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class NetworkActor : public Actor
{
    SHADER_CLASS_HELPER(NetworkActor)
    GENERATED_BODY(NetworkActor, Actor)
    
public:
    NetworkActor(const std::wstring& kName);
    virtual ~NetworkActor() override = default;
    
    FORCEINLINE int GetPacketID() const { return packet_id_; }

    FORCEINLINE bool IsMine() const { return is_mine_; }

protected:
    friend class NetworkManager;
    
    int packet_id_;
    
    bool is_mine_;

    Math::Vector2 received_position_;
    
};

#pragma once
#include "NetworkActor.h"

class ServerActor : public NetworkActor
{
    SHADER_CLASS_HELPER(ServerActor)
    GENERATED_BODY(ServerActor, NetworkActor)
    
public:
    struct Snapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        
        float server_time;
    };
    
    ServerActor(const std::wstring& name);
    virtual ~ServerActor() override = default;

protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void ReceivePacket(Net::IPacket* packet) override;

    std::deque<Snapshot> snapshots_;
    
};

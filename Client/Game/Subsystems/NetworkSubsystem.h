#pragma once
#include "Subsystems/Tickable.h"
#include "Subsystems/WorldSubsystem.h"

namespace Net
{
    struct IPacket;
}

class NetworkSubsystem : public WorldSubsystem, public Tickable
{
    GENERATED_BODY(NetworkSubsystem, WorldSubsystem)
    
public:
    NetworkSubsystem();
    virtual ~NetworkSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    virtual void Tick(float delta_time) override;

private:
    void ProcessPackets(const std::shared_ptr<Net::IPacket>& packet);
    
};

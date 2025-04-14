#pragma once
#include "Subsystems/Tickable.h"
#include "Subsystems/WorldSubsystem.h"

class NetworkSubsystem : public WorldSubsystem, public Tickable
{
    GENERATED_BODY(NetworkSubsystem, WorldSubsystem)
    
public:
    NetworkSubsystem();
    virtual ~NetworkSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    virtual void Tick(float delta_time) override;
    
};

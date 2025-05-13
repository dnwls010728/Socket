#pragma once
#include "Subsystem.h"

class WorldSubsystem : public Subsystem
{
    GENERATED_BODY(WorldSubsystem, Subsystem)
    
public:
    WorldSubsystem();
    virtual ~WorldSubsystem() override = default;

    virtual void OnWorldBeginPlay();
    
};

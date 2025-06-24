#pragma once
#include "Subsystem/WorldSubsystem.h"

class ObjectPoolSubsystem : public WorldSubsystem
{
    GENERATED_BODY(ObjectPoolSubsystem, WorldSubsystem)
    
public:
    ObjectPoolSubsystem();
    virtual ~ObjectPoolSubsystem() override = default;

    virtual void OnWorldBeginPlay() override;
    
};

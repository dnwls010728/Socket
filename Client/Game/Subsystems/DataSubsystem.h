#pragma once
#include "Subsystems/GameInstanceSubsystem.h"

class DataSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(DataSubsystem, GameInstanceSubsystem)
    
public:
    DataSubsystem();
    virtual ~DataSubsystem() override = default;
    
};

#pragma once
#include "Subsystems/Subsystem.h"

class GameInstanceSubsystem : public Subsystem
{
    GENERATED_BODY(GameInstanceSubsystem, Subsystem)
    
public:
    GameInstanceSubsystem();
    virtual ~GameInstanceSubsystem() override = default;
    
};

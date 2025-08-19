#pragma once
#include "Subsystem/GameInstanceSubsystem.h"

class GameSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(GameSubsystem, GameInstanceSubsystem)
    
public:
    GameSubsystem();
    virtual ~GameSubsystem() override = default;

    virtual void Init() override;
    
};

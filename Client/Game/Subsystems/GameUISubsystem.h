#pragma once
#include "Subsystems/GameInstanceSubsystem.h"

class GameUISubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(GameUISubsystem, GameInstanceSubsystem)
    
public:
    GameUISubsystem();
    virtual ~GameUISubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    
};

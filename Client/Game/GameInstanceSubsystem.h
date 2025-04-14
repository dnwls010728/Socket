#pragma once

class GameInstanceSubsystem
{
    GENERATED_BODY(GameInstanceSubsystem)
    
public:
    GameInstanceSubsystem();
    virtual ~GameInstanceSubsystem() = default;

    virtual void Init();
    virtual void Deinit();
    
};

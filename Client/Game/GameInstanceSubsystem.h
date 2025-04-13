#pragma once

class GameInstanceSubsystem : public std::enable_shared_from_this<GameInstanceSubsystem>
{
    SHADER_CLASS_HELPER(GameInstanceSubsystem)
    GENERATED_BODY(GameInstanceSubsystem)
    
public:
    GameInstanceSubsystem();
    virtual ~GameInstanceSubsystem() = default;

    virtual void Init();
    virtual void Deinit();
    
};

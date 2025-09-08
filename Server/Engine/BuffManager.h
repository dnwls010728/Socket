#pragma once

class PlayerCharacter;

class BuffManager
{
public:
    BuffManager(PlayerCharacter* owner);
    ~BuffManager() = default;

    void Tick(float delta_time);

private:
    PlayerCharacter* owner_;
    
};

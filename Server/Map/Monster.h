#pragma once
#include <cstdint>

class GameMap;

class Monster
{
public:
    Monster();
    ~Monster() = default;

private:
    uint32_t unique_id_;
    
    GameMap* map_;
    
};

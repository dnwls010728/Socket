#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "GameMap.h"
#include "../Singleton.h"

class MapManager : public Singleton<MapManager>
{
public:
    MapManager();
    virtual ~MapManager() override = default;

    GameMap* GetMap(uint32_t map_unique_id);

private:
    std::unordered_map<uint32_t, std::unique_ptr<GameMap>> maps_;
    
};

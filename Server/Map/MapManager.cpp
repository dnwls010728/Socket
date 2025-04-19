#include "MapManager.h"

MapManager::MapManager() :
    maps_()
{
}

GameMap* MapManager::GetMap(uint32_t map_unique_id)
{
    auto [it, inserted] = maps_.emplace(map_unique_id, std::make_unique<GameMap>(map_unique_id));
    return it->second.get();
}

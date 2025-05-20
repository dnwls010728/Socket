#include "World.h"

World::World() :
    maps_()
{
}

Map* World::GetMap(uint32_t map_id)
{
    auto [it, inserted] = maps_.emplace(map_id, std::make_unique<Map>(map_id));
    return it->second.get();
}

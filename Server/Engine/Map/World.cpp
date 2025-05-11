#include "pch.h"
#include "World.h"
#include <chrono>

using namespace std::chrono;

World::World()
{
}

World::~World()
{
    Stop();
}

Map* World::GetMap(uint32_t map_unique_id)
{
    std::lock_guard<std::mutex> lock(maps_mutex_);
    auto [it, inserted] = maps_.emplace(map_unique_id, std::make_unique<Map>(map_unique_id));
    Map* map = it->second.get();
    if (inserted && dispatcher_.running())
        dispatcher_.AddMap(map);
    return map;
}

void World::Start(uint32_t tick_interval_ms, size_t maps_per_thread)
{
    if (dispatcher_.running())
        return;
    
    dispatcher_.Start(tick_interval_ms, maps_per_thread);

    std::lock_guard<std::mutex> lock(maps_mutex_);
    for (auto& [map_unique_id, map] : maps_)
    {
        dispatcher_.AddMap(map.get());
    }
}

void World::Stop()
{
    if (dispatcher_.running() == false)
        return;
    
    dispatcher_.Stop();
}
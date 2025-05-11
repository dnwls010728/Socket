#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>

#include "MapTickDispatcher.h"

#include "Map.h"
#include "Engine/Singleton.h"

class World : public Singleton<World>
{
public:
    World();
    virtual ~World();

    Map* GetMap(uint32_t map_unique_id);

    void Start(uint32_t tick_interval_ms, size_t maps_per_thread);
    void Stop();

private:
    std::unordered_map<uint32_t, std::unique_ptr<Map>> maps_;
    std::mutex maps_mutex_;

    MapTickDispatcher dispatcher_;
};

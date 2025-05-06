#pragma once
#include "MapBase.h"
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

namespace Net
{
    struct IPacket;
}

class Map : public MapBase
{
public:
    Map(uint32_t map_id);
    ~Map() = default;
    
    // virtual void Tick(float delta_time);
    // virtual void Physics(float delta_time);
    // virtual void PostTick(float delta_time);

};

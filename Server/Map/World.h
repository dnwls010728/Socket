#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Map.h"
#include "../Singleton.h"

class World : public Singleton<World>
{
public:
    World();
    virtual ~World() override = default;

    Map* GetMap(uint32_t map_unique_id);

private:
    std::unordered_map<uint32_t, std::unique_ptr<Map>> maps_;
    
};

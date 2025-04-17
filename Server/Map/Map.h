#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "../Actors/Actor.h"

class Map
{
public:
    Map();
    virtual ~Map() = default;

    void SpawnActor(const std::shared_ptr<Actor>& actor);

private:
    std::mutex mutex_;
    std::map<Actor::Type, std::vector<std::shared_ptr<Actor>>> actors_;
    
};

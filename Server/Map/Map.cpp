#include "Map.h"

static std::atomic<uint32_t> g_actor_id = 1;

uint32_t GenerateActorID()
{
    return g_actor_id.fetch_add(1);
}

Map::Map() :
    mutex_(),
    actors_()
{
}

void Map::SpawnActor(const std::shared_ptr<Actor>& actor)
{
    std::lock_guard<std::mutex> lock(mutex_);
    actor->SetActorID(GenerateActorID());
    actors_[actor->GetActorType()].push_back(actor);
}

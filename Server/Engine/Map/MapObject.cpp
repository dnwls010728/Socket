#include "pch.h"
#include "MapObject.h"

#include "Map.h"
#include "World.h"

MapObject::MapObject() :
    object_id_(0),
    map_(nullptr),
    position_(Math::Vector2::Zero())
{
}

void MapObject::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
}

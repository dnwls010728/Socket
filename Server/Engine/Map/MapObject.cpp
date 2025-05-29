#include "pch.h"
#include "MapObject.h"

#include "Map.h"

MapObject::MapObject() :
    object_id_(0),
    map_(nullptr),
    position_(Math::Vector2::Zero())
{
}

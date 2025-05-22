#pragma once
#include <cstdint>

#include "Math/Vector2.h"

class MapObject
{
public:
    MapObject();
    virtual ~MapObject() = default;

private:
    uint32_t object_id_;

    Math::Vector2 position_;
    
};

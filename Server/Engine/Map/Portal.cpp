#include "pch.h"
#include "Portal.h"

Portal::Portal(int32_t id, int32_t to_id, int32_t to_map, const Math::Vector2& position) :
    id_(id),
    to_id_(to_id),
    to_map_(to_map),
    position_(position)
{
}

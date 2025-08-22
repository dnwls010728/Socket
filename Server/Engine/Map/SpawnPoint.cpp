#include "pch.h"
#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(uint32_t mob_id, const Math::Vector2& position) :
    mob_id_(mob_id),
    position_(position)
{
}

#include "pch.h"
#include "Item.h"

Item::Item(uint32_t id, uint32_t slot, int32_t count) :
    id_(id),
    slot_(slot),
    count_(count)
{
}

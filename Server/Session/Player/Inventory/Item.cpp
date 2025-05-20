#include "Item.h"

Item::Item(uint32_t item_id, uint32_t slot_index, uint32_t count) :
    item_id_(item_id),
    slot_index_(slot_index),
    count_(count)
{
}

#include "Inventory.h"

Inventory::Inventory() :
    slots_()
{
}

void Inventory::AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    if (slot_index == 0 || item_id == 0) return;
    slots_[slot_index] = { item_id, count };
}

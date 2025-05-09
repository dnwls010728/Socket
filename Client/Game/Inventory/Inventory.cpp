#include "pch.h"
#include "Inventory.h"

#include <ranges>

Inventory::Inventory() :
    inventory_(),
    color_(0)
{
}

int32_t Inventory::GetItemID(uint16_t slot_index) const
{
    auto it = inventory_.find(slot_index);
    if (it != inventory_.end())
        return it->second.item_id;
    
    return -1;
}

uint16_t Inventory::FindItem(int32_t item_id) const
{
    for (const auto& iter : inventory_)
    {
        if (iter.second.item_id == item_id)
            return iter.first;
    }
    
    return -1;
}

uint16_t Inventory::FindFreeSlot() const
{
    uint16_t counter = 1;

    for (const auto& iter : inventory_)
    {
        if (iter.first != counter)
            return counter;

        counter++;
    }

    return counter;
}

int16_t Inventory::GetItemCount(uint16_t slot_index) const
{
    auto it = inventory_.find(slot_index);
    if (it != inventory_.end())
        return it->second.count;

    return 0;
}

int16_t Inventory::GetTotalItemCount(int32_t item_id) const
{
    int16_t total_count = 0;
    for (const auto& slot : inventory_ | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void Inventory::AddSlot(uint16_t slot_index, int32_t item_id, int16_t count)
{
    inventory_[slot_index] = { item_id, count };
}

void Inventory::ChangeCount(uint16_t slot_index, int16_t count)
{
    auto it = inventory_.find(slot_index);
    if (it != inventory_.end())
        it->second.count = count;
}

#include "pch.h"
#include "InventoryData.h"

#include <ranges>

InventoryData::InventoryData() :
    slots_(),
    color_(0)
{
}

int32_t InventoryData::GetItemID(int32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.item_id;
    
    return -1;
}

int32_t InventoryData::FindItem(int32_t item_id) const
{
    for (const auto& iter : slots_)
    {
        if (iter.second.item_id == item_id)
            return iter.first;
    }
    
    return -1;
}

int32_t InventoryData::FindFreeSlot() const
{
    int32_t counter = 1;

    for (const auto& iter : slots_)
    {
        if (iter.first != counter)
            return counter;

        counter++;
    }

    return counter;
}

int16_t InventoryData::GetItemCount(int32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.count;

    return 0;
}

int16_t InventoryData::GetTotalItemCount(int32_t item_id) const
{
    int16_t total_count = 0;
    for (const auto& slot : slots_ | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void InventoryData::AddSlot(int32_t slot_index, int32_t item_id, int16_t count)
{
    slots_[slot_index] = { item_id, count };
}

void InventoryData::ChangeCount(int32_t slot_index, int16_t count)
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        it->second.count = count;
}

void InventoryData::Swap(int32_t first_slot, int32_t second_slot)
{
    Slot first = std::move(slots_[first_slot]);
    slots_[first_slot] = std::move(slots_[second_slot]);
    slots_[second_slot] = std::move(first);

    if (slots_[first_slot].item_id == -1) Remove(first_slot);
    if (slots_[second_slot].item_id == -1) Remove(second_slot);
}

void InventoryData::Remove(int32_t slot_index)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;
    
    slots_.erase(it);
}

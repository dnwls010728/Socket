#include "pch.h"
#include "InventoryManager.h"

#include <ranges>

InventoryManager::InventoryManager() :
    slots_(),
    color_(0)
{
}

uint32_t InventoryManager::GetItemID(uint32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.item_id;
    
    return 0;
}

uint32_t InventoryManager::FindItem(uint32_t item_id) const
{
    for (const auto& iter : slots_)
    {
        if (iter.second.item_id == item_id)
            return iter.first;
    }
    
    return 0;
}

uint32_t InventoryManager::FindFreeSlot() const
{
    uint32_t counter = 1;

    for (const auto& iter : slots_)
    {
        if (iter.first != counter)
            return counter;

        counter++;
    }

    return counter;
}

uint32_t InventoryManager::GetItemCount(uint32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.count;

    return 0;
}

uint32_t InventoryManager::GetTotalItemCount(uint32_t item_id) const
{
    uint32_t total_count = 0;
    for (const auto& slot : slots_ | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void InventoryManager::AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    slots_[slot_index] = { item_id, count };
}

void InventoryManager::ChangeCount(uint32_t slot_index, uint32_t count)
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        it->second.count = count;
}

void InventoryManager::Swap(uint32_t first_slot, uint32_t second_slot)
{
    Slot first = std::move(slots_[first_slot]);
    slots_[first_slot] = std::move(slots_[second_slot]);
    slots_[second_slot] = std::move(first);

    if (slots_[first_slot].item_id == -1) Remove(first_slot);
    if (slots_[second_slot].item_id == -1) Remove(second_slot);
}

void InventoryManager::Remove(uint32_t slot_index)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;
    
    slots_.erase(it);
}

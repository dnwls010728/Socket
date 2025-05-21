#include "Inventory.h"

#include <ranges>

Inventory::Inventory() :
    slots_()
{
}

uint32_t Inventory::GetItemID(uint32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(uint32_t item_id) const
{
    for (const auto& it : slots_)
    {
        if (it.second.item_id == item_id)
            return it.first;
    }
    
    return 0;
}

uint32_t Inventory::FindFreeSlot() const
{
    uint32_t counter = 1;

    for (const auto& it : slots_)
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    return counter;
}

uint32_t Inventory::GetItemCount(uint32_t slot_index) const
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.count;

    return 0;
}

uint32_t Inventory::GetTotalItemCount(uint32_t item_id) const
{
    uint32_t total_count = 0;
    for (const auto& slot : slots_ | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void Inventory::AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    if (slot_index == 0 || item_id == 0) return;
    slots_[slot_index] = { item_id, slot_index, count };
}

void Inventory::ChangeCount(uint32_t slot_index, uint32_t count)
{
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        it->second.count = count;
}

void Inventory::Swap(uint32_t first_slot, uint32_t second_slot)
{
    Slot first = std::move(slots_[first_slot]);
    slots_[first_slot] = std::move(slots_[second_slot]);
    slots_[second_slot] = std::move(first);

    if (!slots_[first_slot].item_id) Remove(first_slot);
    if (!slots_[second_slot].item_id) Remove(second_slot);
}

void Inventory::Remove(uint32_t slot_index)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;
    
    slots_.erase(it);
}

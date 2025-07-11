#include "pch.h"
#include "Inventory.h"

#include <ranges>

#include "Subsystems/Publisher/PublisherSubsystem.h"

Inventory::Inventory() :
    slots_(),
    color_(0),
    next_unique_id_(0)
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

uint32_t Inventory::AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    slots_[slot_index] = { ++next_unique_id_, item_id, count };
    return next_unique_id_;
}

void Inventory::ChangeCount(uint32_t slot_index, uint32_t count)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;

    it->second.count = count;

    ItemCountChangedEventData event_data;
    event_data.slot = slot_index;
    event_data.count = count;
        
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemCountChanged, event_data);
}

void Inventory::Swap(uint32_t first_slot, uint32_t second_slot)
{
    Slot first = std::move(slots_[first_slot]);
    slots_[first_slot] = std::move(slots_[second_slot]);
    slots_[second_slot] = std::move(first);

    if (!slots_[first_slot].item_id) Remove(first_slot);
    if (!slots_[second_slot].item_id) Remove(second_slot);

    ItemSwappedEventData event_data;
    event_data.first_slot = first_slot;
    event_data.second_slot = second_slot;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemSwapped, event_data);
}

void Inventory::Remove(uint32_t slot_index)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;
    
    slots_.erase(it);

    ItemRemovedEventData event_data;
    event_data.slot = slot_index;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemRemoved, event_data);
}

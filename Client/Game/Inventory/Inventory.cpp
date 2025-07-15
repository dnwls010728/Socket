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

uint32_t Inventory::GetItemID(Type type, uint32_t slot_index)
{
    auto it = slots_[type].find(slot_index);
    if (it != slots_[type].end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(Type type, uint32_t item_id)
{
    for (const auto& it : slots_[type])
    {
        if (it.second.item_id == item_id)
            return it.first;
    }
    
    return 0;
}

uint32_t Inventory::FindFreeSlot(Type type)
{
    uint32_t counter = 1;

    for (const auto& it : slots_[type])
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    return counter;
}

uint32_t Inventory::GetItemCount(Type type, uint32_t slot_index)
{
    auto it = slots_[type].find(slot_index);
    if (it != slots_[type].end())
        return it->second.count;

    return 0;
}

uint32_t Inventory::GetTotalItemCount(Type type, uint32_t item_id)
{
    uint32_t total_count = 0;
    for (const auto& slot : slots_[type] | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

uint32_t Inventory::AddSlot(Type type, uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    slots_[type][slot_index] = { ++next_unique_id_, item_id, count };
    return next_unique_id_;
}

void Inventory::ChangeCount(Type type, uint32_t slot_index, uint32_t count)
{
    auto it = slots_[type].find(slot_index);
    if (it == slots_[type].end()) return;

    it->second.count = count;

    ItemCountChangedEventData event_data;
    event_data.slot = slot_index;
    event_data.count = count;
        
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemCountChanged, event_data);
}

void Inventory::Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot)
{
    Slot first = std::move(slots_[first_type][first_slot]);
    slots_[first_type][first_slot] = std::move(slots_[second_type][second_slot]);
    slots_[second_type][second_slot] = std::move(first);

    if (!slots_[first_type][first_slot].item_id) Remove(first_type, first_slot);
    if (!slots_[second_type][second_slot].item_id) Remove(second_type, second_slot);

    ItemSwappedEventData event_data;
    event_data.first_slot = first_slot;
    event_data.second_slot = second_slot;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemSwapped, event_data);
}

void Inventory::Remove(Type type, uint32_t slot_index)
{
    auto it = slots_[type].find(slot_index);
    if (it == slots_[type].end()) return;
    
    slots_[type].erase(it);

    ItemRemovedEventData event_data;
    event_data.slot = slot_index;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemRemoved, event_data);
}

#include "pch.h"
#include "Inventory.h"

#include <ranges>

#include "Subsystems/Publisher/PublisherSubsystem.h"

Inventory::Inventory() :
    inventories_(),
    slot_capacity_(),
    color_(0),
    next_id_(0)
{
}

uint32_t Inventory::GetItemID(Type type, uint32_t slot_index)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(Type type, uint32_t item_id)
{
    for (const auto& it : inventories_[static_cast<uint8_t>(type)])
    {
        if (it.second.item_id == item_id)
            return it.first;
    }
    
    return 0;
}

uint32_t Inventory::FindFreeSlot(Type type) const
{
    uint32_t counter = 1;

    for (const auto& it : inventories_[static_cast<uint8_t>(type)])
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    return (counter <= slot_capacity_[static_cast<uint8_t>(type)]) ? counter : 0;
}

int32_t Inventory::GetItemCount(Type type, uint32_t slot_index)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.count;

    return 0;
}

int32_t Inventory::GetTotalItemCount(Type type, uint32_t item_id)
{
    int32_t total_count = 0;
    for (const auto& slot : inventories_[static_cast<uint8_t>(type)] | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

uint32_t Inventory::AddSlot(Type type, uint32_t slot_index, uint32_t item_id, int32_t count)
{
    inventories_[static_cast<uint8_t>(type)][slot_index] = { ++next_id_, item_id, count };
    return next_id_;
}

void Inventory::ChangeCount(Type type, uint32_t slot_index, int32_t count)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;

    it->second.count = count;

    ItemCountChangedEventData event_data;
    event_data.slot = slot_index;
    event_data.count = count;
        
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemCountChanged, event_data);
}

void Inventory::Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot)
{
    Slot first = std::move(inventories_[static_cast<uint8_t>(first_type)][first_slot]);
    inventories_[static_cast<uint8_t>(first_type)][first_slot] = std::move(inventories_[static_cast<uint8_t>(second_type)][second_slot]);
    inventories_[static_cast<uint8_t>(second_type)][second_slot] = std::move(first);

    if (!inventories_[static_cast<uint8_t>(first_type)][first_slot].item_id) Remove(first_type, first_slot);
    if (!inventories_[static_cast<uint8_t>(second_type)][second_slot].item_id) Remove(second_type, second_slot);

    ItemSwappedEventData event_data;
    event_data.first_slot = first_slot;
    event_data.second_slot = second_slot;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemSwapped, event_data);
}

void Inventory::Remove(Type type, uint32_t slot_index)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;
    
    inventories_[static_cast<uint8_t>(type)].erase(it);

    ItemRemovedEventData event_data;
    event_data.slot = slot_index;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemRemoved, event_data);
}

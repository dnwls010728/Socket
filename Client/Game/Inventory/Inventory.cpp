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

uint32_t Inventory::GetItemID(InventoryType type, uint32_t slot_id)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindFreeSlot(InventoryType type) const
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

int32_t Inventory::GetItemCount(InventoryType type, uint32_t slot_id)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.count;

    return 0;
}

int32_t Inventory::GetTotalItemCount(InventoryType type, uint32_t item_id)
{
    int32_t total_count = 0;
    for (const auto& slot : inventories_[static_cast<uint8_t>(type)] | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

uint32_t Inventory::AddSlot(InventoryType type, uint32_t slot_id, uint32_t item_id, int32_t count)
{
    inventories_[static_cast<uint8_t>(type)][slot_id] = { ++next_id_, item_id, count };

    ItemAddedData event_data;
    event_data.inventory_type = type;
    event_data.slot_id = slot_id;
    event_data.item_id = item_id;
    event_data.count = count;
    
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemAdded, event_data);
    return next_id_;
}

uint32_t Inventory::FindItemSlotID(InventoryType type, uint32_t item_id) const
{
    for (const auto& it : inventories_[static_cast<uint8_t>(type)])
    {
        if (it.second.item_id == item_id)
            return it.first;
    }

    return 0;
}

void Inventory::ChangeCount(InventoryType type, uint32_t slot_id, int32_t count)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;

    it->second.count = count;

    ItemCountChangedData event_data;
    event_data.inventory_type = type;
    event_data.slot_id = slot_id;
    event_data.count = count;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemCountChanged, event_data);
}

void Inventory::Swap(InventoryType first_type, uint32_t first_slot, InventoryType second_type, uint32_t second_slot)
{
    Slot first = inventories_[static_cast<uint8_t>(first_type)][first_slot];
    inventories_[static_cast<uint8_t>(first_type)][first_slot] = inventories_[static_cast<uint8_t>(second_type)][second_slot];
    inventories_[static_cast<uint8_t>(second_type)][second_slot] = first;

    if (inventories_[static_cast<uint8_t>(first_type)][first_slot].item_id == 0) Remove(first_type, first_slot);
    if (inventories_[static_cast<uint8_t>(second_type)][second_slot].item_id == 0) Remove(second_type, second_slot);

    ItemMovedData event_data;
    event_data.first_inventory_type = first_type;
    event_data.first_slot_index = first_slot;
    event_data.second_inventory_type = second_type;
    event_data.second_slot_index = second_slot;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemMoved, event_data);
}

void Inventory::Remove(InventoryType type, uint32_t slot_id)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;
    
    inventories_[static_cast<uint8_t>(type)].erase(it);

    ItemRemovedData event_data;
    event_data.inventory_type = type;
    event_data.slot_id = slot_id;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kItemRemoved, event_data);
}

void Inventory::SetColor(int32_t color)
{
    color_ = color;

    ColorUpdateData event_data;
    event_data.color = color;
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kColorUpdated, event_data);
}

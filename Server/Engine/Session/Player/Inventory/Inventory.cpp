#include "pch.h"
#include "Inventory.h"

#include <CustomPacket.h>
#include <ranges>

#include "DataManager.h"
#include "Item.h"
#include "Map/PlayerCharacter.h"

Inventory::Inventory(PlayerCharacter* owner, InventoryType type) :
    owner_(owner),
    type_(type),
    items_(),
    capacity_(0),
    mutex_()
{
}

void Inventory::SetItem(uint32_t slot_id, const std::shared_ptr<Item>& item)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (slot_id > capacity_) return;

    item->SetSlot(slot_id);
    items_[slot_id] = item;
}

void Inventory::EraseItem(uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    items_.erase(slot_id);
}

void Inventory::Move(uint32_t first_slot, uint32_t second_slot)
{
}

void Inventory::Swap(std::shared_ptr<Item>& first, std::shared_ptr<Item>& second)
{
}

std::shared_ptr<Item> Inventory::FindItem(uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = items_.find(slot_id);
    if (it != items_.end())
        return it->second;

    return nullptr;
}

std::vector<std::shared_ptr<Item>> Inventory::FindItems(uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::shared_ptr<Item>> items;
    for (const auto& item : items_ | std::views::values)
    {
        if (item->GetID() == item_id)
            items.push_back(item);
    }

    return items;
}

bool Inventory::AddItem(const std::shared_ptr<Item>& item)
{
    if (!item || IsFull()) return false;
    
    uint32_t item_id = item->GetID();
    int32_t count = item->GetCount();

    const auto& item_data = DataManager::Get()->GetItem(item_id);
    if (!item_data) return false;

    int32_t max_count = item_data->max_count;

    std::vector<std::shared_ptr<Item>> items = FindItems(item_id);
    
    InventoryUpdatePacket packet;

    if (!items.empty())
    {
        auto it = items.begin();
        while (count > 0)
        {
            if (it == items.end()) break;

            const auto& existing_item = *it;
            int32_t existing_count = existing_item->GetCount();
            if (existing_count < max_count)
            {
                int32_t to_add = std::min(count, max_count - existing_count);
                existing_item->SetCount(existing_count + to_add);
                count -= to_add;

                InventoryChange change;
                change.inventory_type = static_cast<uint8_t>(type_);
                change.action = InventoryAction::kChangeCount;
                change.info.change_count.slot_id = existing_item->GetSlot();
                change.info.change_count.count = existing_item->GetCount();
                packet.changes.push_back(change);
            }

            ++it;
        }
    }

    while (count > 0)
    {
        uint32_t free_slot = FindFreeSlot();
        if (free_slot == 0)
        {
            item->SetCount(count);
            return false;
        }

        int32_t to_add = std::min(count, max_count);
        SetItem(free_slot, Item::Create(item_id, to_add));
        count -= to_add;

        InventoryChange change;
        change.inventory_type = static_cast<uint8_t>(type_);
        change.action = InventoryAction::kAdd;
        change.info.add.slot_id = free_slot;
        change.info.add.item_id = item_id;
        change.info.add.count = to_add;
        packet.changes.push_back(change);
    }

    owner_->SendPacket(packet);
    return true;
}

bool Inventory::IsFull()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size() >= capacity_;
}

uint32_t Inventory::FindFreeSlot()
{
    if (IsFull()) return 0;
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    uint32_t next = 1;
    for (const auto& item : items_)
    {
        if (item.first != next) break;
        ++next;
    }

    return next <= capacity_ ? next : 0;
}

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
    if (slot_id > capacity_) return;

    item->SetSlot(slot_id);
    items_[slot_id] = item;
}

void Inventory::EraseItem(uint32_t slot_id)
{
    items_.erase(slot_id);
}

void Inventory::MoveOrStackSlots(uint32_t first_slot, uint32_t second_slot)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto first_it = items_.find(first_slot);
    if (first_it == items_.end()) return;
    
    auto second_it = items_.find(second_slot);
    if (second_it == items_.end())
    {
        auto node = items_.extract(first_it);
        node.key() = second_slot;
        node.mapped()->SetSlot(second_slot);
        items_.insert(std::move(node));

        InventoryUpdatePacket packet;
        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kMove;
            change.move.first_slot = first_slot;
            change.move.second_slot = second_slot;
            packet.changes.push_back(change);
        }
        owner_->SendPacket(packet);
        return;
    }

    auto& first_item = first_it->second;
    auto& second_item = second_it->second;
    
    const auto& item_data = DataManager::Get()->GetItem(first_item->GetID());
    if (!item_data) return;

    if (type_ == InventoryType::kEquip || second_item->GetID() != first_item->GetID())
    {
        SwapSlots(first_slot, second_slot);
        
        InventoryUpdatePacket packet;
        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kMove;
            change.move.first_slot = first_slot;
            change.move.second_slot = second_slot;
            packet.changes.push_back(change);
        }
        owner_->SendPacket(packet);
        return;
    }

    const int32_t first_count = first_item->GetCount();
    const int32_t second_count = second_item->GetCount();
    const int32_t total_count = first_count + second_count;
    const int32_t max_count = item_data->max_count;

    InventoryUpdatePacket packet;

    if (total_count > max_count)
    {
        second_item->SetCount(max_count);
        first_item->SetCount(total_count - max_count);

        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kChangeCount;
            change.change_count.slot_id = first_slot;
            change.change_count.count = first_item->GetCount();
            packet.changes.push_back(change);
        }

        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kChangeCount;
            change.change_count.slot_id = second_slot;
            change.change_count.count = second_item->GetCount();
            packet.changes.push_back(change);
        }

        owner_->SendPacket(packet);
    }
    else
    {
        second_item->SetCount(total_count);
        items_.erase(first_it);
        
        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kChangeCount;
            change.change_count.slot_id = second_slot;
            change.change_count.count = second_item->GetCount();
            packet.changes.push_back(change);
        }

        {
            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kRemove;
            change.remove.slot_id = first_slot;
            packet.changes.push_back(change);
        }
        
        owner_->SendPacket(packet);
    }
}

std::shared_ptr<Item> Inventory::FindItem(uint32_t slot_id)
{
    auto it = items_.find(slot_id);
    if (it != items_.end())
        return it->second;

    return nullptr;
}

std::vector<std::shared_ptr<Item>> Inventory::FindItems(uint32_t item_id)
{
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
    if (!item) return false;
    
    uint32_t item_id = item->GetID();
    int32_t remaining_count = item->GetCount();

    const auto& item_data = DataManager::Get()->GetItem(item_id);
    if (!item_data) return false;

    int32_t max_count = item_data->max_count;
    
    InventoryUpdatePacket packet;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        
        for (auto& it : items_)
        {
            if (remaining_count == 0) break;
        
            const auto& existing_item = it.second;
            if (existing_item->GetID() != item_id) continue;

            const int32_t existing_count = existing_item->GetCount();
            if (existing_count >= max_count) continue;

            const int32_t to_add = std::min(remaining_count, max_count - existing_count);
            existing_item->SetCount(existing_count + to_add);
            remaining_count -= to_add;

            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kChangeCount;
            change.change_count.slot_id = it.first;
            change.change_count.count = existing_item->GetCount();
            packet.changes.push_back(change);
        }

        while (remaining_count > 0)
        {
            uint32_t free_slot = FindFreeSlot();
            if (free_slot == 0) break;

            int32_t to_add = std::min(remaining_count, max_count);
            items_.try_emplace(free_slot, Item::Create(item_id, to_add));
            remaining_count -= to_add;

            InventoryChange change;
            change.inventory_type = static_cast<uint8_t>(type_);
            change.action = InventoryAction::kAdd;
            change.add.slot_id = free_slot;
            change.add.item_id = item_id;
            change.add.count = to_add;
            packet.changes.push_back(change);
        }
    }

    if (!packet.changes.empty())
        owner_->SendPacket(packet);

    if (remaining_count > 0)
    {
        item->SetCount(remaining_count);
        return false;
    }

    return true;
}

bool Inventory::IsFull() const
{
    return items_.size() >= capacity_;
}

uint32_t Inventory::FindFreeSlot() const
{
    if (items_.size() >= capacity_) return 0;
    
    uint32_t next = 1;
    for (const auto& item : items_)
    {
        if (item.first != next) break;
        ++next;
    }

    return next <= capacity_ ? next : 0;
}

void Inventory::SwapSlots(uint32_t first_slot, uint32_t second_slot)
{
    if (first_slot == second_slot) return;
    
    auto first_it = items_.find(first_slot);
    auto second_it = items_.find(second_slot);
    if (first_it == items_.end() || second_it == items_.end()) return;

    std::swap(first_it->second, second_it->second);
    first_it->second->SetSlot(first_slot);
    second_it->second->SetSlot(second_slot);
}

#include "pch.h"
#include "Inventory.h"

#include <CustomPacket.h>
#include <algorithm>
#include <iostream>
#include <ranges>

#include "DataManager.h"
#include "Item.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "MySQL/MySQLManager.h"

Inventory::Inventory(PlayerCharacter* owner) :
    owner_(owner),
    inventories_(),
    slot_capacity_()
{
    slot_capacity_[static_cast<uint8_t>(Type::kEquipped)] = static_cast<uint8_t>(EquipSlot::kCount);
}

uint32_t Inventory::GetItemID(Type type, uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second->GetID();
    
    return 0;
}

uint32_t Inventory::FindFreeSlot(Type type)
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t counter = 1;

    for (const auto& it : inventories_[static_cast<uint8_t>(type)])
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    return (counter <= slot_capacity_[static_cast<uint8_t>(type)]) ? counter : 0;
}

int32_t Inventory::GetItemCount(Type type, uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second->GetCount();

    return 0;
}

int32_t Inventory::GetTotalItemCount(Type type, uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t total_count = 0;
    for (const auto& slot : inventories_[static_cast<uint8_t>(type)] | std::views::values)
    {
        if (slot->GetID() == item_id)
            total_count += slot->GetCount();
    }

    return total_count;
}

std::shared_ptr<Item> Inventory::FindItem(Type type, uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second;

    return nullptr;
}

void Inventory::AddSlot(Type type, uint32_t slot_id, uint32_t item_id, int32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_); // 필요 없을지도...?
    if (slot_id == 0 || item_id == 0) return;
    inventories_[static_cast<uint8_t>(type)][slot_id] = std::make_shared<Item>(item_id, slot_id, count);
}

void Inventory::ChangeCount(Type type, uint32_t slot_id, int32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        it->second->SetCount(count);
}

void Inventory::Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto& first_inventory = inventories_[static_cast<uint8_t>(first_type)];
    auto& second_inventory = inventories_[static_cast<uint8_t>(second_type)];
    
    std::shared_ptr<Item> first = first_inventory[first_slot];
    std::shared_ptr<Item> second = second_inventory[second_slot];

    if (first) first->SetSlot(second_slot);
    if (second) second->SetSlot(first_slot);
    
    first_inventory[first_slot] = second;
    second_inventory[second_slot] = first;

    if (!first_inventory[first_slot]) Remove_Internal(first_type, first_slot);
    if (!second_inventory[second_slot]) Remove_Internal(second_type, second_slot);
}

void Inventory::Remove(Type type, uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Remove_Internal(type, slot_id);
}

void Inventory::RemoveItem(Type type, uint32_t slot_id, int32_t count)
{
    auto item = FindItem(type, slot_id);
    if (!item) return;

    int32_t new_count = std::max(item->GetCount() - count, 0);
    item->SetCount(new_count);
}

void Inventory::GetItems(std::vector<std::shared_ptr<Item>>& out_items, uint32_t item_id) const
{
    uint32_t type_code = item_id / 100000; // 장비 100000, 소비 200000, 기타 300000
    Type type = static_cast<Type>(type_code);

    const auto& inventory = inventories_[static_cast<uint8_t>(type)];
    for (const auto& val : inventory | std::views::values)
    {
        if (val->GetID() == item_id) out_items.push_back(val);
    }
}

bool Inventory::AddItem(const std::shared_ptr<Item>& item)
{
    if (!item) return false;
    
    uint32_t item_id = item->GetID();
    int32_t count = item->GetCount();
    
    const auto& item_data = DataManager::Get()->GetItem(item_id);
    if (!item_data) return false;
    
    int32_t max_count = item_data->max_count;

    uint32_t type_code = item_id / 100000; // 장비 100000, 소비 200000, 기타 300000
    Type type = static_cast<Type>(type_code);

    uint32_t slot_capacity = slot_capacity_[static_cast<uint8_t>(type)];

    std::vector<std::shared_ptr<Item>> items;
    GetItems(items, item_id);

    InventoryUpdatePacket packet;

    if (!items.empty())
    {
        auto it = items.begin();
        while (count > 0)
        {
            if (it == items.end()) break;

            auto& existing_item = *it++;
            int32_t existing_count = existing_item->GetCount();
            if (existing_count < max_count)
            {
                int32_t space_left = max_count - existing_count;
                int32_t to_add = std::min(count, space_left);
                
                existing_item->SetCount(existing_count + to_add);
                count -= to_add;

                InventoryChange change;
                change.inventory_type = static_cast<uint8_t>(type);
                change.action = InventoryAction::kChangeCount;
                change.info.change_count.slot_id = existing_item->GetSlot();
                change.info.change_count.count = existing_item->GetCount();
                packet.changes.push_back(change);
                
                owner_->SendPacket(packet);
            }
        }
    }

    while (count > 0)
    {
        uint32_t slot_id = FindFreeSlot(type);
        if (slot_id == 0 || slot_id > slot_capacity)
        {
            item->SetCount(count);
            return false;
        }

        int32_t to_add = std::min(count, max_count);
        AddSlot(type, slot_id, item_id, to_add);
        count -= to_add;

        InventoryChange change;
        change.inventory_type = static_cast<uint8_t>(type);
        change.action = InventoryAction::kAdd;
        change.info.add.slot_id = slot_id;
        change.info.add.item_id = item_id;
        change.info.add.count = to_add;
        packet.changes.push_back(change);

        owner_->SendPacket(packet);
    }
    
    return true;
}

bool Inventory::UpdateDatabase() const
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return false;

    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("DELETE FROM inventory_item_info WHERE character_id = ?"));
            statement->setUInt(1, owner_->GetObjectID());
            statement->executeUpdate();
        }

        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO inventory_item_info (account_id, character_id, inventory_type, item_id, slot_id, count) VALUES (?, ?, ?, ?, ?, ?)"));
            for (int32_t i = 0; i < static_cast<int32_t>(Type::kCount); ++i)
            {
                for (const auto& slot : inventories_[i])
                {
                    statement->setUInt(1, owner_->GetAccountID());
                    statement->setUInt(2, owner_->GetObjectID());
                    statement->setUInt(3, i);
                    statement->setUInt(4, slot.second->GetID());
                    statement->setUInt(5, slot.first);
                    statement->setInt(6, slot.second->GetCount());
                    statement->executeUpdate();
                }
            }
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return false;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return false;
    }

    return true;
}

void Inventory::Remove_Internal(Type type, uint32_t slot_id)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_id);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;
    
    inventories_[static_cast<uint8_t>(type)].erase(it);
}

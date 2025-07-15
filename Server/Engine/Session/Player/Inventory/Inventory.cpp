#include "pch.h"
#include "Inventory.h"

#include <iostream>
#include <ranges>

#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "MySQL/MySQLManager.h"

Inventory::Inventory(const std::shared_ptr<PlayerCharacter>& player) :
    player_character_(player),
    inventories_()
{
}

uint32_t Inventory::GetItemID(Type type, uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[type].find(slot_index);
    if (it != inventories_[type].end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(Type type, uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& it : inventories_[type])
    {
        if (it.second.item_id == item_id)
            return it.first;
    }
    
    return 0;
}

uint32_t Inventory::FindFreeSlot(Type type)
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t counter = 1;

    for (const auto& it : inventories_[type])
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    // 인벤토리 슬롯의 최대 개수를 초과하지 않는지 확인 필요
    return counter;
}

uint32_t Inventory::GetItemCount(Type type, uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[type].find(slot_index);
    if (it != inventories_[type].end())
        return it->second.count;

    return 0;
}

uint32_t Inventory::GetTotalItemCount(Type type, uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t total_count = 0;
    for (const auto& slot : inventories_[type] | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void Inventory::AddSlot(Type type, uint32_t slot_index, uint32_t item_id, uint32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (slot_index == 0 || item_id == 0) return;
    inventories_[type][slot_index] = { item_id, count };
}

void Inventory::ChangeCount(Type type, uint32_t slot_index, uint32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[type].find(slot_index);
    if (it != inventories_[type].end())
        it->second.count = count;
}

void Inventory::Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Slot first = std::move(inventories_[first_type][first_slot]);
    inventories_[first_type][first_slot] = std::move(inventories_[second_type][second_slot]);
    inventories_[second_type][second_slot] = std::move(first);

    if (!inventories_[first_type][first_slot].item_id) Remove_Internal(first_type, first_slot);
    if (!inventories_[second_type][second_slot].item_id) Remove_Internal(second_type, second_slot);
}

void Inventory::Remove(Type type, uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Remove_Internal(type, slot_index);
}

void Inventory::Update()
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return;
    return; // 임시 처리
    if (auto player_character = player_character_.lock())
    {
        try
        {
            {
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("DELETE FROM inventory_item_info WHERE character_id = ?"));
                statement->setUInt(1, player_character->GetObjectID());
                statement->executeUpdate();
            }

            {
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO inventory_item_info (account_id, character_id, item_id, slot_index, count) VALUES (?, ?, ?, ?, ?)"));
                for (const auto& it : inventories_)
                {
                    statement->setUInt(1, player_character->GetAccountID());
                    statement->setUInt(2, player_character->GetObjectID());
                    // statement->setUInt(3, it.second.item_id);
                    // statement->setUInt(4, it.first);
                    // statement->setUInt(5, it.second.count);
                    statement->executeUpdate();
                }
            }
        }
        catch (sql::SQLException& e)
        {
            std::cerr << "SQLException: " << e.what() << std::endl;
            std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
            std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown Exception" << std::endl;
        }
    }
}

void Inventory::Remove_Internal(Type type, uint32_t slot_index)
{
    auto it = inventories_[type].find(slot_index);
    if (it == inventories_[type].end()) return;
    
    inventories_[type].erase(it);
}

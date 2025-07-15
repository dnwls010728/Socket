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
    slots_()
{
}

uint32_t Inventory::GetItemID(uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& it : slots_)
    {
        if (it.second.item_id == item_id)
            return it.first;
    }
    
    return 0;
}

uint32_t Inventory::FindFreeSlot()
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t counter = 1;

    for (const auto& it : slots_)
    {
        if (it.first != counter)
            return counter;

        counter++;
    }

    // 인벤토리 슬롯의 최대 개수를 초과하지 않는지 확인 필요
    return counter;
}

uint32_t Inventory::GetItemCount(uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        return it->second.count;

    return 0;
}

uint32_t Inventory::GetTotalItemCount(uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
    if (slot_index == 0 || item_id == 0) return;
    slots_[slot_index] = { item_id, count };
}

void Inventory::ChangeCount(uint32_t slot_index, uint32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = slots_.find(slot_index);
    if (it != slots_.end())
        it->second.count = count;
}

void Inventory::Swap(uint32_t first_slot, uint32_t second_slot)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Slot first = std::move(slots_[first_slot]);
    slots_[first_slot] = std::move(slots_[second_slot]);
    slots_[second_slot] = std::move(first);

    if (!slots_[first_slot].item_id) Remove_Internal(first_slot);
    if (!slots_[second_slot].item_id) Remove_Internal(second_slot);
}

void Inventory::Remove(uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Remove_Internal(slot_index);
}

void Inventory::Update()
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return;

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
                for (const auto& it : slots_)
                {
                    statement->setUInt(1, player_character->GetAccountID());
                    statement->setUInt(2, player_character->GetObjectID());
                    statement->setUInt(3, it.second.item_id);
                    statement->setUInt(4, it.first);
                    statement->setUInt(5, it.second.count);
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

void Inventory::Remove_Internal(uint32_t slot_index)
{
    auto it = slots_.find(slot_index);
    if (it == slots_.end()) return;
    
    slots_.erase(it);
}

#include "pch.h"
#include "Inventory.h"

#include <CustomPacket.h>
#include <iostream>
#include <ranges>

#include "DataManager.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "MySQL/MySQLManager.h"

Inventory::Inventory(const std::shared_ptr<PlayerCharacter>& player) :
    player_character_(player),
    inventories_(),
    slot_capacity_()
{
}

uint32_t Inventory::GetItemID(Type type, uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.item_id;
    
    return 0;
}

uint32_t Inventory::FindItem(Type type, uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& it : inventories_[static_cast<uint8_t>(type)])
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
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        return it->second.count;

    return 0;
}

int32_t Inventory::GetTotalItemCount(Type type, uint32_t item_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t total_count = 0;
    for (const auto& slot : inventories_[static_cast<uint8_t>(type)] | std::views::values)
    {
        if (slot.item_id == item_id)
            total_count += slot.count;
    }

    return total_count;
}

void Inventory::AddSlot(Type type, uint32_t slot_index, uint32_t item_id, int32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_); // 필요 없을지도...?
    if (slot_index == 0 || item_id == 0) return;
    inventories_[static_cast<uint8_t>(type)][slot_index] = { slot_index, item_id, count };
}

void Inventory::ChangeCount(Type type, uint32_t slot_index, int32_t count)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it != inventories_[static_cast<uint8_t>(type)].end())
        it->second.count = count;
}

void Inventory::Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Slot first = inventories_[static_cast<uint8_t>(first_type)][first_slot];
    inventories_[static_cast<uint8_t>(first_type)][first_slot] = inventories_[static_cast<uint8_t>(second_type)][second_slot];
    inventories_[static_cast<uint8_t>(second_type)][second_slot] = first;

    if (!inventories_[static_cast<uint8_t>(first_type)][first_slot].item_id) Remove_Internal(first_type, first_slot);
    if (!inventories_[static_cast<uint8_t>(second_type)][second_slot].item_id) Remove_Internal(second_type, second_slot);
}

void Inventory::Remove(Type type, uint32_t slot_index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    Remove_Internal(type, slot_index);
}

bool Inventory::IsFull(Type type)
{
    std::lock_guard<std::mutex> lock(mutex_);

    uint64_t used_slots = inventories_[static_cast<uint8_t>(type)].size();
    uint64_t max_slots = slot_capacity_[static_cast<uint8_t>(type)];
    return used_slots >= max_slots;
}

bool Inventory::IsFull(Type type, uint32_t margin)
{
    std::lock_guard<std::mutex> lock(mutex_);

    uint64_t used_slots = inventories_[static_cast<uint8_t>(type)].size();
    uint64_t max_slots = slot_capacity_[static_cast<uint8_t>(type)];
    return (used_slots + margin) >= max_slots;
}

bool Inventory::HasSpace(uint32_t item_id, int32_t count)
{
    Type item_type = Type::kNone;

    uint32_t item_type_code = item_id / 100000;
    if (item_type_code >= 1 && item_type_code <= 3)
        item_type = static_cast<Type>(item_type_code);

    int32_t remaining_count = count;
    if (item_type != Type::kEquip)
    {
        int32_t max_count = 0;
        if (const auto* item_data = DataManager::Get()->GetItem(item_id))
            max_count = item_data->max_count;
        
        std::vector<Slot*> slots = GetSlotsWithItem(item_type, item_id);
        if (!slots.empty())
        {
            for (const auto& slot : slots)
            {
                int32_t old_count = slot->count;
                if (old_count < max_count)
                {
                    int32_t new_count = std::min(old_count + remaining_count, max_count);
                    remaining_count -= (new_count - old_count);
                }
                
                if (remaining_count <= 0) break;
            }
        }
        
        int32_t slots_needed = 1;
        if (max_count > 1)
            slots_needed = static_cast<int32_t>((static_cast<int64_t>(remaining_count) + max_count - 1) / max_count);
        
        return !IsFull(item_type, slots_needed);
    }

    return !IsFull(item_type);
}

bool Inventory::AddItem(uint32_t item_id, int32_t count)
{
    Type item_type = Type::kNone;

    uint32_t item_type_code = item_id / 100000;
    if (item_type_code >= 1 && item_type_code <= 3)
        item_type = static_cast<Type>(item_type_code);

    int32_t remaining_count = count;
    if (item_type != Type::kEquip)
    {
        int32_t max_count = 0;
        if (const auto* item_data = DataManager::Get()->GetItem(item_id))
            max_count = item_data->max_count;
        
        std::vector<Slot*> slots = GetSlotsWithItem(item_type, item_id);
        if (!slots.empty())
        {
            auto it = slots.begin();
            while (remaining_count > 0)
            {
                if (it != slots.end())
                {
                    Slot* slot = *it++;
                    int32_t old_count = slot->count;
                    if (old_count < max_count)
                    {
                        int32_t new_count = std::min(old_count + remaining_count, max_count);
                        remaining_count -= (new_count - old_count);
                        slot->count = new_count;

                        ChangeItemCountPacket packet;
                        packet.inventory_type = static_cast<uint8_t>(item_type);
                        packet.slot_index = slot->slot_index;
                        packet.count = new_count;
                        
                        if (auto player_character = player_character_.lock()) player_character->SendPacket(packet);
                    }
                }
                else break;
            }
        }

        while (remaining_count > 0)
        {
            int32_t new_count = std::min(remaining_count, max_count);
            remaining_count -= new_count;
            
            uint32_t free_slot = FindFreeSlot(item_type);
            if (free_slot == 0)
            {
                // 인벤토리가 가득 찼습니다.
                return false;
            }
            
            AddSlot(item_type, free_slot, item_id, new_count);

            AddItemPacket packet;
            packet.inventory_type = static_cast<uint8_t>(item_type);
            packet.slot_index = free_slot;
            packet.item_id = item_id;
            packet.count = new_count;
            
            if (auto player_character = player_character_.lock()) player_character->SendPacket(packet);
        }
    }
    else
    {
        uint32_t free_slot = FindFreeSlot(item_type);
        if (free_slot == 0)
        {
            // 인벤토리가 가득 찼습니다.
            return false;
        }
        
        AddSlot(item_type, free_slot, item_id, remaining_count);
        
        AddItemPacket packet;
        packet.inventory_type = static_cast<uint8_t>(item_type);
        packet.slot_index = free_slot;
        packet.item_id = item_id;
        packet.count = remaining_count;
        
        if (auto player_character = player_character_.lock()) player_character->SendPacket(packet);
    }

    return true;
}

bool Inventory::UpdateDatabase() const
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return false;
    
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
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO inventory_item_info (account_id, character_id, inventory_type, item_id, slot_index, count) VALUES (?, ?, ?, ?, ?, ?)"));
                for (int32_t i = 0; i < static_cast<int32_t>(Type::kCount); ++i)
                {
                    for (const auto& slot : inventories_[i])
                    {
                        statement->setUInt(1, player_character->GetAccountID());
                        statement->setUInt(2, player_character->GetObjectID());
                        statement->setUInt(3, i);
                        statement->setUInt(4, slot.second.item_id);
                        statement->setUInt(5, slot.first);
                        statement->setInt(6, slot.second.count);
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

    return false;
}

std::vector<Inventory::Slot*> Inventory::GetSlotsWithItem(Type type, uint32_t item_id)
{
    std::vector<Slot*> out_slots;
    auto& inventory = inventories_[static_cast<uint8_t>(type)];
    for (auto& val : inventory | std::views::values)
    {
        if (val.item_id == item_id) out_slots.push_back(&val);
    }
    
    return out_slots;
}

void Inventory::Remove_Internal(Type type, uint32_t slot_index)
{
    auto it = inventories_[static_cast<uint8_t>(type)].find(slot_index);
    if (it == inventories_[static_cast<uint8_t>(type)].end()) return;
    
    inventories_[static_cast<uint8_t>(type)].erase(it);
}

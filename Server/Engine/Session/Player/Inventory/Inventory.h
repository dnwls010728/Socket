#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <array>
#include <vector>

class Item;
class PlayerCharacter;

class Inventory
{
public:
    enum class Type : uint8_t
    {
        kNone = 0,
        kEquip,
        kUse,
        kEtc,
        kEquipped,
        kCount
    };
    
    Inventory(const std::shared_ptr<PlayerCharacter>& owner);
    ~Inventory() = default;
    
    uint32_t GetItemID(Type type, uint32_t slot_id);

    uint32_t FindFreeSlot(Type type);
    
    int32_t GetItemCount(Type type, uint32_t slot_id);
    int32_t GetTotalItemCount(Type type, uint32_t item_id);

    std::shared_ptr<Item> FindItem(Type type, uint32_t slot_id);

    void AddSlot(Type type, uint32_t slot_id, uint32_t item_id, int32_t count);
    void ChangeCount(Type type, uint32_t slot_id, int32_t count);
    void Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot);
    void Remove(Type type, uint32_t slot_id);
    void GetItems(std::vector<std::shared_ptr<Item>>& out_items, uint32_t item_id) const;

    bool AddItem(const std::shared_ptr<Item>& item);
    bool UpdateDatabase() const;

    inline void SetSlotCapacity(Type type, uint32_t capacity) { slot_capacity_[static_cast<uint8_t>(type)] = capacity; }
    inline uint32_t GetSlotCapacity(Type type) const { return slot_capacity_[static_cast<uint8_t>(type)]; }

    inline const std::array<std::map<uint32_t, std::shared_ptr<Item>>, static_cast<uint8_t>(Type::kCount)>& GetInventories() const { return inventories_; }

private:
    void Remove_Internal(Type type, uint32_t slot_id);
    
    std::weak_ptr<PlayerCharacter> player_character_;
    
    std::array<std::map<uint32_t, std::shared_ptr<Item>>, static_cast<uint8_t>(Type::kCount)> inventories_;
    std::array<uint32_t, static_cast<uint8_t>(Type::kCount)> slot_capacity_;

    std::mutex mutex_;
    
};

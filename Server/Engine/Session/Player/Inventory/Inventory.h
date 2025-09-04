#pragma once
#include <CommonObject.h>
#include <map>
#include <mutex>
#include <vector>

class Item;
class PlayerCharacter;

class Inventory
{
public:
    Inventory(PlayerCharacter* owner, InventoryType type);
    ~Inventory() = default;

    void SetItem(uint32_t slot_id, const std::shared_ptr<Item>& item);
    void EraseItem(uint32_t slot_id);
    void Move(uint32_t first_slot, uint32_t second_slot);
    void Swap(uint32_t first_slot, uint32_t second_slot);

    std::shared_ptr<Item> FindItem(uint32_t slot_id);

    std::vector<std::shared_ptr<Item>> FindItems(uint32_t item_id);

    bool AddItem(const std::shared_ptr<Item>& item);
    bool IsFull();

    uint32_t FindFreeSlot();

    inline const std::map<uint32_t, std::shared_ptr<Item>>& GetItems() const { return items_; }

    inline void SetCapacity(uint32_t capacity) { capacity_ = capacity; }
    inline uint32_t GetCapacity() const { return capacity_; }

private:
    bool IsFull_Internal() const;
    
    uint32_t FindFreeSlot_Internal() const;
    
    PlayerCharacter* owner_;
    InventoryType type_;
    std::map<uint32_t, std::shared_ptr<Item>> items_;
    uint32_t capacity_;
    std::mutex mutex_;
    
};

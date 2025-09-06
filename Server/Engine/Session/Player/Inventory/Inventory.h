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
    void MoveOrStackSlots(uint32_t first_slot, uint32_t second_slot);

    std::shared_ptr<Item> EraseItem(uint32_t slot_id);
    std::shared_ptr<Item> FindItem(uint32_t slot_id);

    std::vector<std::shared_ptr<Item>> FindItems(uint32_t item_id);

    bool AddItem(const std::shared_ptr<Item>& item);
    bool IsFull() const;

    uint32_t FindFreeSlot() const;

    inline const std::map<uint32_t, std::shared_ptr<Item>>& GetItems() const { return items_; }

    inline void SetCapacity(uint32_t capacity) { capacity_ = capacity; }
    inline uint32_t GetCapacity() const { return capacity_; }

    inline std::unique_lock<std::mutex> Lock() { return std::unique_lock<std::mutex>(mutex_); }
    inline std::unique_lock<std::mutex> DeferLock() { return std::unique_lock<std::mutex>(mutex_, std::defer_lock); }

    // auto begin() { return items_.begin(); }
    // auto end() { return items_.end(); }

private:
    void SwapSlots(uint32_t first_slot, uint32_t second_slot);
    
    PlayerCharacter* owner_;
    InventoryType type_;
    std::map<uint32_t, std::shared_ptr<Item>> items_;
    uint32_t capacity_;
    std::mutex mutex_;
    
};

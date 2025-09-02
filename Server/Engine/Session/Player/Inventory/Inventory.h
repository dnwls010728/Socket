#pragma once
#include <map>
#include <mutex>

class Item;
class PlayerCharacter;

class Inventory
{
public:
    Inventory(PlayerCharacter* owner);
    ~Inventory() = default;

    void AddItem(uint32_t slot_id, const std::shared_ptr<Item>& item);

    bool IsFull();

    inline void SetCapacity(uint32_t capacity) { capacity_ = capacity; }
    inline uint32_t GetCapacity() const { return capacity_; }

private:
    PlayerCharacter* owner_;
    std::map<uint32_t, std::shared_ptr<Item>> items_;
    uint32_t capacity_;
    std::mutex mutex_;
    
};

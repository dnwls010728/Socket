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

    void SetItemAt(uint32_t slot_id, const std::shared_ptr<Item>& item);
    void Swap(const std::shared_ptr<Item>& first, const std::shared_ptr<Item>& second);

    int32_t GetCount(uint32_t slot_id);

    bool IsFull();

    inline void SetCapacity(uint32_t capacity) { capacity_ = capacity; }
    inline uint32_t GetCapacity() const { return capacity_; }

private:
    PlayerCharacter* owner_;
    std::map<uint32_t, std::shared_ptr<Item>> items_;
    uint32_t capacity_;
    std::mutex mutex_;
    
};

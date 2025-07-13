#pragma once
#include <map>
#include <memory>
#include <mutex>

class PlayerCharacter;

class Inventory
{
public:
    struct Slot
    {
        uint32_t item_id;
        uint32_t count;
    };
    
    Inventory(const std::shared_ptr<PlayerCharacter>& owner);
    ~Inventory() = default;
    
    uint32_t GetItemID(uint32_t slot_index);

    uint32_t FindItem(uint32_t item_id);
    uint32_t FindFreeSlot();
    uint32_t GetItemCount(uint32_t slot_index);
    uint32_t GetTotalItemCount(uint32_t item_id);

    void AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count);
    void ChangeCount(uint32_t slot_index, uint32_t count);
    void Swap(uint32_t first_slot, uint32_t second_slot);
    void Remove(uint32_t slot_index);
    
    void Update();

private:
    std::weak_ptr<PlayerCharacter> player_character_;
    
    std::map<uint32_t, Slot> slots_;

    std::mutex mutex_;

public:
    inline const std::map<uint32_t, Slot>& GetSlots() const { return slots_; }
    
};

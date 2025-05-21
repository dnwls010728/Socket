#pragma once
#include <map>
#include <memory>
#include <vector>

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;
    
    uint32_t GetItemID(uint32_t slot_index) const;

    uint32_t FindItem(uint32_t item_id) const;
    uint32_t FindFreeSlot() const;
    uint32_t GetItemCount(uint32_t slot_index) const;
    uint32_t GetTotalItemCount(uint32_t item_id) const;

    void AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count);
    void ChangeCount(uint32_t slot_index, uint32_t count);
    void Swap(uint32_t first_slot, uint32_t second_slot);
    void Remove(uint32_t slot_index);

private:
    struct Slot
    {
        uint32_t item_id;
        uint32_t slot_index;
        uint32_t count;
    };
    
    std::map<uint32_t, Slot> slots_;

public:
    inline const std::map<uint32_t, Slot>& GetSlots() const { return slots_; }
    
};

#pragma once
#include <map>
#include <memory>

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;

    void AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count);

private:
    struct Slot
    {
        uint32_t item_id;
        uint32_t count;
    };
    
    std::map<uint32_t, Slot> slots_;
    
};

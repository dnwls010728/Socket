#pragma once
#include <cstdint>

class Item
{
public:
    Item(uint32_t item_id, uint32_t slot_index, uint32_t count);
    virtual ~Item() = default;

    inline uint32_t GetItemID() const { return item_id_; }
    
    inline uint32_t GetSlotIndex() const { return slot_index_; }
    inline void SetSlotIndex(uint32_t slot_index) { slot_index_ = slot_index; }
    
    inline uint32_t GetCount() const { return count_; }
    inline void SetCount(uint32_t count) { count_ = count; }

protected:
    uint32_t item_id_;
    uint32_t slot_index_;
    uint32_t count_;
    
};

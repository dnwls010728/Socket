#pragma once

class InventoryManager
{
public:
    InventoryManager();
    ~InventoryManager() = default;
    
    uint32_t GetItemID(uint32_t slot_index) const;

    uint32_t FindItem(uint32_t item_id) const;
    uint32_t FindFreeSlot() const;
    uint32_t GetItemCount(uint32_t slot_index) const;
    uint32_t GetTotalItemCount(uint32_t item_id) const;
    
    void AddSlot(uint32_t slot_index, uint32_t item_id, uint32_t count);
    void ChangeCount(uint32_t slot_index, uint32_t count);
    void Swap(uint32_t first_slot, uint32_t second_slot);
    void Remove(uint32_t slot_index);

    FORCEINLINE void SetColor(uint32_t color) { color_ = color; }
    FORCEINLINE uint32_t GetColor() const { return color_; }

private:
    struct Slot
    {
        uint32_t item_id = -1;
        uint32_t count = 0;
    };
    
    std::map<uint32_t, Slot> slots_;

    // 재화
    uint32_t color_;
    
};

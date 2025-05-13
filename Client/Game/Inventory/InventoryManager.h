#pragma once

class InventoryManager
{
public:
    InventoryManager();
    ~InventoryManager() = default;
    
    int32_t GetItemID(int32_t slot_index) const;

    int32_t FindItem(int32_t item_id) const;
    int32_t FindFreeSlot() const;
    int16_t GetItemCount(int32_t slot_index) const;
    int16_t GetTotalItemCount(int32_t item_id) const;
    
    void AddSlot(int32_t slot_index, int32_t item_id, int16_t count);
    void ChangeCount(int32_t slot_index, int16_t count);
    void Swap(int32_t first_slot, int32_t second_slot);
    void Remove(int32_t slot_index);

    FORCEINLINE void SetColor(int64_t color) { color_ = color; }
    FORCEINLINE int64_t GetColor() const { return color_; }

private:
    struct Slot
    {
        int32_t item_id = -1;
        int16_t count = 0;
    };
    
    std::map<int32_t, Slot> slots_;

    // 재화
    int64_t color_;
    
};

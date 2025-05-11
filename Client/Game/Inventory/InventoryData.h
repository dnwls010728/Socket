#pragma once

class InventoryData
{
public:
    InventoryData();
    ~InventoryData() = default;
    
    int32_t GetItemID(uint16_t slot_index) const;

    uint16_t FindItem(int32_t item_id) const;
    uint16_t FindFreeSlot() const;
    int16_t GetItemCount(uint16_t slot_index) const;
    int16_t GetTotalItemCount(int32_t item_id) const;
    
    void AddSlot(uint16_t slot_index, int32_t item_id, int16_t count);
    void ChangeCount(uint16_t slot_index, int16_t count);
    void Swap(uint16_t first_slot, uint16_t second_slot);
    void Remove(uint16_t slot_index);

    FORCEINLINE void SetColor(int64_t color) { color_ = color; }
    FORCEINLINE int64_t GetColor() const { return color_; }

private:
    struct Slot
    {
        int32_t item_id = -1;
        int16_t count = 0;

        FORCEINLINE bool IsValid() const { return item_id != -1; }
    };
    
    std::map<uint16_t, Slot> slots_;

    // 재화
    int64_t color_;
    
};

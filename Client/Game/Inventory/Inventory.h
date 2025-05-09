#pragma once

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;
    
    int32_t GetItemID(uint16_t slot_index) const;

    uint16_t FindItem(int32_t item_id) const;
    uint16_t FindFreeSlot() const;
    int16_t GetItemCount(uint16_t slot_index) const;
    int16_t GetTotalItemCount(int32_t item_id) const;
    
    void AddSlot(uint16_t slot_index, int32_t item_id, int16_t count);
    void ChangeCount(uint16_t slot_index, int16_t count);

    FORCEINLINE void SetColor(int64_t color) { color_ = color; }
    FORCEINLINE int64_t GetColor() const { return color_; }

private:
    struct Slot
    {
        int32_t item_id;
        int16_t count;
    };
    
    // X: 슬롯 인덱스 % 행 개수
    // Y: 슬롯 인덱스 / 행 개수
    std::map<uint16_t, Slot> inventory_;

    // 재화
    int64_t color_;
    
};

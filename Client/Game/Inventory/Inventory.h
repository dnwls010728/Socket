#pragma once

class Inventory
{
public:
    enum class Type : uint8_t
    {
        kNone = 0,
        kEquip,
        kUse,
        kEtc,
        kEquipped,
        kCount
    };
    
    Inventory();
    ~Inventory() = default;
    
    uint32_t GetItemID(Type type, uint32_t slot_index);

    uint32_t FindItem(Type type, uint32_t item_id);
    uint32_t FindFreeSlot(Type type) const;
    
    int32_t GetItemCount(Type type, uint32_t slot_index);
    int32_t GetTotalItemCount(Type type, uint32_t item_id);
    
    uint32_t AddSlot(Type type, uint32_t slot_index, uint32_t item_id, int32_t count);
    
    void ChangeCount(Type type, uint32_t slot_index, int32_t count);
    void Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot);
    void Remove(Type type, uint32_t slot_index);
    void SetColor(int32_t color);

    FORCEINLINE void SetSlotCapacity(Type type, uint32_t capacity) { slot_capacity_[static_cast<uint8_t>(type)] = capacity; }
    FORCEINLINE uint32_t GetSlotCapacity(Type type) const { return slot_capacity_[static_cast<uint8_t>(type)]; }

    FORCEINLINE int32_t GetColor() const { return color_; }

private:
    struct Slot
    {
        uint32_t id;
        uint32_t item_id;
        
        int32_t count;
    };
    
    std::array<std::map<uint32_t, Slot>, static_cast<uint8_t>(Type::kCount)> inventories_;
    std::array<uint32_t, static_cast<uint8_t>(Type::kCount)> slot_capacity_;

    // 재화
    int32_t color_;

    uint32_t next_id_;
    
};

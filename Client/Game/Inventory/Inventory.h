#pragma once
#include <CustomPacket.h>

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;
    
    uint32_t GetItemID(InventoryType type, uint32_t slot_id);

    uint32_t FindFreeSlot(InventoryType type) const;
    
    int32_t GetItemCount(InventoryType type, uint32_t slot_id);
    int32_t GetTotalItemCount(InventoryType type, uint32_t item_id);
    
    uint32_t AddSlot(InventoryType type, uint32_t slot_id, uint32_t item_id, int32_t count);
    uint32_t FindItemSlotID(InventoryType type, uint32_t item_id) const;
    
    void ChangeCount(InventoryType type, uint32_t slot_id, int32_t count);
    void Swap(InventoryType first_type, uint32_t first_slot, InventoryType second_type, uint32_t second_slot);
    void Remove(InventoryType type, uint32_t slot_id);
    void SetColor(int32_t color);

    FORCEINLINE void SetSlotCapacity(InventoryType type, uint32_t capacity) { slot_capacity_[static_cast<uint8_t>(type)] = capacity; }
    FORCEINLINE uint32_t GetSlotCapacity(InventoryType type) const { return slot_capacity_[static_cast<uint8_t>(type)]; }

    FORCEINLINE int32_t GetColor() const { return color_; }

private:
    struct Slot
    {
        uint32_t id;
        uint32_t item_id;
        
        int32_t count;
    };
    
    std::array<std::map<uint32_t, Slot>, static_cast<uint8_t>(InventoryType::kCount)> inventories_;
    std::array<uint32_t, static_cast<uint8_t>(InventoryType::kCount)> slot_capacity_;

    // 재화
    int32_t color_;

    uint32_t next_id_;
    
};

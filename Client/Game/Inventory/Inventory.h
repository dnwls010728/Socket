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
        kCount
    };
    
    Inventory();
    ~Inventory() = default;
    
    uint32_t GetItemID(Type type, uint32_t slot_index);

    uint32_t FindItem(Type type, uint32_t item_id);
    uint32_t FindFreeSlot(Type type);
    
    int32_t GetItemCount(Type type, uint32_t slot_index);
    int32_t GetTotalItemCount(Type type, uint32_t item_id);
    
    uint32_t AddSlot(Type type, uint32_t slot_index, uint32_t item_id, int32_t count);
    
    void ChangeCount(Type type, uint32_t slot_index, int32_t count);
    void Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot);
    void Remove(Type type, uint32_t slot_index);

    FORCEINLINE void SetColor(int32_t color) { color_ = color; }
    FORCEINLINE int32_t GetColor() const { return color_; }

private:
    struct Slot
    {
        uint32_t unique_id;
        uint32_t item_id;
        
        int32_t count;
    };
    
    std::unordered_map<Type, std::map<uint32_t, Slot>> inventories_;

    // 재화
    int32_t color_;

    uint32_t next_unique_id_;
    
};

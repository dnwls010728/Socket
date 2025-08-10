#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <array>
#include <vector>

class PlayerCharacter;

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
    
    struct Slot
    {
        uint32_t slot_index;
        uint32_t item_id;
        
        int32_t count;
    };
    
    Inventory(const std::shared_ptr<PlayerCharacter>& owner);
    ~Inventory() = default;
    
    uint32_t GetItemID(Type type, uint32_t slot_index);

    uint32_t FindItem(Type type, uint32_t item_id);
    uint32_t FindFreeSlot(Type type);
    
    int32_t GetItemCount(Type type, uint32_t slot_index);
    int32_t GetTotalItemCount(Type type, uint32_t item_id);

    void AddSlot(Type type, uint32_t slot_index, uint32_t item_id, int32_t count);
    void ChangeCount(Type type, uint32_t slot_index, int32_t count);
    void Swap(Type first_type, uint32_t first_slot, Type second_type, uint32_t second_slot);
    void Remove(Type type, uint32_t slot_index);
    
    bool IsFull(Type type);
    bool IsFull(Type type, uint32_t margin);
    bool HasSpace(uint32_t item_id, int32_t count);
    bool AddItem(uint32_t item_id, int32_t count);
    bool UpdateDatabase() const;
    
    std::vector<Slot*> GetSlotsWithItem(Type type, uint32_t item_id);

    inline void SetSlotCapacity(Type type, uint32_t capacity) { slot_capacity_[static_cast<uint8_t>(type)] = capacity; }
    inline uint32_t GetSlotCapacity(Type type) const { return slot_capacity_[static_cast<uint8_t>(type)]; }

    inline const std::array<std::map<uint32_t, Slot>, static_cast<uint8_t>(Type::kCount)>& GetInventories() const { return inventories_; }

private:
    void Remove_Internal(Type type, uint32_t slot_index);
    
    std::weak_ptr<PlayerCharacter> player_character_;
    
    std::array<std::map<uint32_t, Slot>, static_cast<uint8_t>(Type::kCount)> inventories_;
    std::array<uint32_t, static_cast<uint8_t>(Type::kCount)> slot_capacity_;

    std::mutex mutex_;
    
};

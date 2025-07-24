#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>

class PlayerCharacter;

class Inventory
{
public:
    enum class Type : uint8_t
    {
        kNone = 0,
        kEquip,
        kUse,
        kEtc
    };
    
    struct Slot
    {
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
    
    void Update();

    inline const std::unordered_map<Type, std::map<uint32_t, Slot>>& GetInventories() const { return inventories_; }

private:
    void Remove_Internal(Type type, uint32_t slot_index);
    
    std::weak_ptr<PlayerCharacter> player_character_;
    
    std::unordered_map<Type, std::map<uint32_t, Slot>> inventories_;

    std::mutex mutex_;
    
};

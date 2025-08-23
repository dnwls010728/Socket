#pragma once
#include "Inventory/Inventory.h"
#include "PacketHandlers/PlayerStatsUpdateHandler.h"
#include "Subsystem/GameInstanceSubsystem.h"

struct EventData
{
    virtual ~EventData() = default;
};

struct StatUpdateData : EventData
{
    PlayerStat stat;
    int32_t value;
    
    virtual ~StatUpdateData() override = default;
};

struct ItemAddedData : EventData
{
    Inventory::Type inventory_type;
    uint32_t slot_index;
    uint32_t item_id;
    int32_t count;
    
    virtual ~ItemAddedData() override = default;
};

struct ItemCountChangedData : EventData
{
    Inventory::Type inventory_type;
    uint32_t slot_index;
    int32_t count;
    
    virtual ~ItemCountChangedData() override = default;
};

struct ItemMovedData : EventData
{
    Inventory::Type first_inventory_type;
    uint32_t first_slot_index;
    Inventory::Type second_inventory_type;
    uint32_t second_slot_index;
    
    virtual ~ItemMovedData() override = default;
};

struct ItemRemovedData : EventData
{
    Inventory::Type inventory_type;
    uint32_t slot_index;
    
    virtual ~ItemRemovedData() override = default;
};

struct ColorUpdateData : EventData
{
    int32_t color;
    
    virtual ~ColorUpdateData() override = default;
};

DECLARE_DELEGATE(OnPublisherDelegate, const EventData&)

class PublisherSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(PublisherSubsystem, GameInstanceSubsystem)
    
public:
    enum class EventType : uint8_t
    {
        kNone = 0,
        kStatUpdated,
        kItemAdded,
        kItemCountChanged,
        kItemMoved,
        kItemRemoved,
        kColorUpdated
    };
    
    PublisherSubsystem();
    virtual ~PublisherSubsystem() override = default;

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void Subscribe(EventType type, M* obj, void (M::*func)(const EventData&));

    template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void Unsubscribe(EventType type, M* obj, void (M::*func)(const EventData&));

    void Publish(EventType type, const EventData& data);

    static PublisherSubsystem* Get();

private:
    std::map<EventType, OnPublisherDelegate> events_;
    
    
};

template <typename M, typename>
void PublisherSubsystem::Subscribe(EventType type, M* obj, void(M::* func)(const EventData&))
{
    events_[type].Add(obj, func);
}

template <typename M, typename>
void PublisherSubsystem::Unsubscribe(EventType type, M* obj, void(M::* func)(const EventData&))
{
    auto it = events_.find(type);
    if (it != events_.end())
    {
        it->second.Remove(obj, func);
    }
}

#pragma once
#include "Subsystem/GameInstanceSubsystem.h"

struct EventData
{
    virtual ~EventData() = default;
};

struct HPChangedEventData : EventData
{
    int32_t hp;
    int32_t max_hp;

    virtual ~HPChangedEventData() override = default;
};

struct ExpChangedEventData : EventData
{
    int32_t exp;

    virtual ~ExpChangedEventData() override = default;
};

struct LvChangedEventData : EventData
{
    int32_t lv;

    virtual ~LvChangedEventData() override = default;
};

struct ItemSwappedEventData : EventData
{
    uint32_t first_slot;
    uint32_t second_slot;

    virtual ~ItemSwappedEventData() override = default;
};

struct ItemCountChangedEventData : EventData
{
    uint32_t slot;
    uint32_t count;

    virtual ~ItemCountChangedEventData() override = default;
};

struct ItemRemovedEventData : EventData
{
    uint32_t slot;

    virtual ~ItemRemovedEventData() override = default;
};

DECLARE_DELEGATE(OnPublisherDelegate, const EventData&)

class PublisherSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(PublisherSubsystem, GameInstanceSubsystem)
    
public:
    enum class EventType : uint8_t
    {
        kNone = 0,
        kHPChanged,
        kExpChanged,
        kLvChanged,
        kItemSwapped,
        kItemCountChanged,
        kItemRemoved
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

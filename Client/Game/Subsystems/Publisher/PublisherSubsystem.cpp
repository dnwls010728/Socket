#include "pch.h"
#include "PublisherSubsystem.h"

#include "GameInstance.h"

PublisherSubsystem::PublisherSubsystem() :
    events_()
{
}

void PublisherSubsystem::Publish(EventType type, const EventData& data)
{
    auto it = events_.find(type);
    if (it != events_.end()) it->second.ExecuteIfBound(data);
}

PublisherSubsystem* PublisherSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PublisherSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PublisherSubsystem>("PublisherSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}

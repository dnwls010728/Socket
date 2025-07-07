#include "pch.h"
#include "PlayerSubsystem.h"

#include "GameInstance.h"
#include "Publisher/PublisherSubsystem.h"

PlayerSubsystem::PlayerSubsystem() :
    account_id_(0),
    character_id_(0),
    hp_(0),
    max_hp_(0),
    exp_(0),
    name_(L""),
    initial_position_x_(0.f),
    initial_position_y_(0.f),
    inventory_(nullptr)
{
}

void PlayerSubsystem::SetHP(uint32_t hp)
{
    hp_ = hp;

    HPChangedEventData event_data;
    event_data.hp = hp_;
    event_data.max_hp = max_hp_;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kHPChanged, event_data);
}

void PlayerSubsystem::SetMaxHP(uint32_t max_hp)
{
    max_hp_ = max_hp;

    HPChangedEventData event_data;
    event_data.hp = hp_;
    event_data.max_hp = max_hp_;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kHPChanged, event_data);
}

void PlayerSubsystem::SetExp(uint32_t exp)
{
    exp_ = exp;

    ExpChangedEventData event_data;
    event_data.exp = exp_;

    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kExpChanged, event_data);
}

PlayerSubsystem* PlayerSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PlayerSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerSubsystem>("PlayerSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}

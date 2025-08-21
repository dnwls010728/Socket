#include "pch.h"
#include "PlayerSubsystem.h"

#include "GameInstance.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Publisher/PublisherSubsystem.h"

PlayerSubsystem::PlayerSubsystem() :
    account_id_(0),
    character_id_(0),
    map_id_(0),
    lv_(0),
    hp_(0),
    max_hp_(0),
    exp_(0),
    name_(L"Unknown"),
    body_color_(L"FFFFFF"),
    profiles_(),
    inventory_(nullptr),
    portal_cooldown_(0.f)
{
}

void PlayerSubsystem::UpdateStat(PlayerStat stat, int32_t value)
{
    switch (stat)
    {
    case PlayerStat::kHP:
        {
            hp_ = value;

            HPChangedEventData event_data;
            event_data.hp = hp_;
            event_data.max_hp = max_hp_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kHPChanged, event_data);
        }
        break;
    case PlayerStat::kMaxHP:
        {
            max_hp_ = value;

            HPChangedEventData event_data;
            event_data.hp = hp_;
            event_data.max_hp = max_hp_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kHPChanged, event_data);
        }
        break;
    case PlayerStat::kExp:
        {
            exp_ = value;

            ExpChangedEventData event_data;
            event_data.exp = exp_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kExpChanged, event_data);
        }
        break;
    case PlayerStat::kLv:
        {
            Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\levelup.mp3");
            AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
            
            lv_ = value;

            LvChangedEventData event_data;
            event_data.lv = lv_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kLvChanged, event_data);
        }
        break;
    }

    Logger::Print(L"PlayerSubsystem::UpdateStat: Updated stat %d to value %u", static_cast<int>(stat), value);
}

void PlayerSubsystem::AddProfile(const CharacterProfile& profile)
{
    profiles_.push_back(profile);
}

void PlayerSubsystem::DeleteProfile(uint32_t character_id)
{
    for (auto it = profiles_.begin(); it != profiles_.end(); ++it)
    {
        if (it->character_id == character_id)
        {
            profiles_.erase(it);
            return;
        }
    }
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

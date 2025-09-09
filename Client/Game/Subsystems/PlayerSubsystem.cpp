#include "pch.h"
#include "PlayerSubsystem.h"

#include "GameInstance.h"
#include "PostProcessingSettings.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Math/Math.h"
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

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kHP;
            event_data.value = hp_;
            
            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kMaxHP:
        {
            max_hp_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kMaxHP;
            event_data.value = max_hp_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kExp:
        {
            exp_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kExp;
            event_data.value = exp_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kLv:
        {
            Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\levelup.mp3");
            AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
            
            lv_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kLv;
            event_data.value = lv_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kAtk:
        {
            atk_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kAtk;
            event_data.value = atk_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kDef:
        {
            def_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kDef;
            event_data.value = def_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
        }
        break;
    case PlayerStat::kDig:
        {
            dig_ = value;

            StatUpdateData event_data;
            event_data.stat = PlayerStat::kDig;
            event_data.value = dig_;

            PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kStatUpdated, event_data);
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

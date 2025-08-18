#include "pch.h"
#include "ObjectSpawnHandler.h"

#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/DroppedItem.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Mobs/MobBase.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool ObjectSpawnHandler::Handle(Net::IPacket* packet)
{
    ObjectSpawnPacket* received_packet = dynamic_cast<ObjectSpawnPacket*>(packet);
    if (!received_packet) return false;

    const ObjectInfo& object_info = received_packet->object_info;

    switch (object_info.type)
    {
    case ObjectType::kPlayer:
        {
            NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();
            std::shared_ptr<PlayerCharacter> player_character = network_subsystem->SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), object_info.object_id);
            if (IsValid(player_character))
            {
                player_character->Init(object_info.info.player.name, object_info.info.player.body_color, {object_info.position_x, object_info.position_y});
                network_subsystem->other_players_.emplace_back(player_character);
            }
        }
        break;
    case ObjectType::kMob:
        {
            std::shared_ptr<Actor> out_actor = nullptr;
            if (!ObjectPoolSubsystem::Get()->GetFromPool(MobBase::StaticClass(), out_actor)) return false;
            if (auto mob = std::dynamic_pointer_cast<MobBase>(out_actor))
            {
                mob->SetObjectID(object_info.object_id);
                mob->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y });
                
                mob->Init(object_info.info.mob.mob_id);
            }
        }
        break;

    case ObjectType::kDroppedItem:
        {
            std::shared_ptr<Actor> out_actor = nullptr;
            if (!ObjectPoolSubsystem::Get()->GetFromPool(DroppedItem::StaticClass(), out_actor)) return false;
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(out_actor))
            {
                if (!object_info.info.dropped_item.color)
                {
                    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SFX\\itemdrop.mp3");
                    AudioManager::Get()->PlaySound2D(audio);
                }
                else
                {
                    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SFX\\moneydrop.mp3");
                    AudioManager::Get()->PlaySound2D(audio);
                }
                
                DroppedItemInfo info = object_info.info.dropped_item;

                Math::Vector2 offset = Math::Vector2::Up() * .5f;
                
                dropped_item->SetObjectID(object_info.object_id);
                dropped_item->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y + offset.y });

                dropped_item->Init(info.item_id, info.color, { object_info.position_x, object_info.position_y + offset.y });
            }
        }
        break;
    }
    
    return true;
}

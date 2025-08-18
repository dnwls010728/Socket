#include "pch.h"
#include "ObjectDestroyHandler.h"

#include <CustomPacket.h>

#include "Actors/DroppedItem.h"
#include "Actors/NetworkActor.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool ObjectDestroyHandler::Handle(Net::IPacket* packet)
{
    ObjectDestroyPacket* received_packet = dynamic_cast<ObjectDestroyPacket*>(packet);
    if (!received_packet) return false;

    NetworkSubsystem* subsystem = NetworkSubsystem::Get();

    const ObjectDestroyInfo& info = received_packet->object_info;
    auto network_actor = subsystem->FindNetworkActor(received_packet->object_info.object_id);

    switch (info.type)
    {
    case ObjectType::kPlayer:
        {
            subsystem->DestroyNetworkActor(received_packet->object_info.object_id);
        }
        break;

    case ObjectType::kMob:
        {
            if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
        }
        break;
        
    case ObjectType::kDroppedItem:
        {
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(network_actor))
            {
                Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SFX\\PickUpItem.mp3");
                AudioManager::Get()->PlayOneShot(audio);
                
                auto player_character = subsystem->FindNetworkActor(info.info.dropped_item.character_id);
                dropped_item->Pickup(player_character);
            }
        }
        break;

    default:
        {
            subsystem->DestroyNetworkActor(received_packet->object_info.object_id);
        }
        break;
    }
    
    return true;
}

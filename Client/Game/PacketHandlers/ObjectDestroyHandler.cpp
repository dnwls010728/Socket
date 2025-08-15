#include "pch.h"
#include "ObjectDestroyHandler.h"

#include <CustomPacket.h>

#include "Actors/DroppedItem.h"
#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool ObjectDestroyHandler::Handle(Net::IPacket* packet)
{
    ObjectDestroyPacket* received_packet = dynamic_cast<ObjectDestroyPacket*>(packet);
    if (!received_packet) return false;

    const ObjectDestroyInfo& info = received_packet->object_info;
    auto network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_info.object_id);

    switch (info.type)
    {
    case ObjectType::kDroppedItem:
        {
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(network_actor))
            {
                auto player_character = NetworkSubsystem::Get()->FindNetworkActor(info.info.dropped_item.character_id);
                dropped_item->Pickup(player_character);
            }
        }
        break;

    default:
        {
            if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
        }
        break;
    }
    
    return true;
}

#include "pch.h"
#include "ObjectDestroyHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool ObjectDestroyHandler::Handle(Net::IPacket* packet)
{
    ObjectDestroyPacket* received_packet = dynamic_cast<ObjectDestroyPacket*>(packet);
    if (!received_packet) return false;

    const ObjectDestroyInfo& info = received_packet->object_info;
    switch (info.type)
    {
    case ObjectType::kDroppedItem:
        {
            std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_info.object_id);
            if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
        }
        break;

    default:
        {
            std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_info.object_id);
            if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
        }
        break;
    }
    
    return true;
}

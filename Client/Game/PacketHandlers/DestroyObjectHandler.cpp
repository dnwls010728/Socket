#include "pch.h"
#include "DestroyObjectHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool DestroyObjectHandler::Handle(Net::IPacket* packet)
{
    DestroyObjectPacket* received_packet = dynamic_cast<DestroyObjectPacket*>(packet);
    if (!received_packet) return false;

    std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_id);
    if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
    
    return true;
}

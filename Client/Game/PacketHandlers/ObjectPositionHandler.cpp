#include "pch.h"
#include "ObjectPositionHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool ObjectPositionHandler::Handle(Net::IPacket* packet)
{
    ObjectPositionPacket* received_packet = dynamic_cast<ObjectPositionPacket*>(packet);
    if (!received_packet) return false;

    std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_id);
    if (!IsValid(network_actor)) return false;

    network_actor->ReceivePacket(received_packet);
    return true;
}

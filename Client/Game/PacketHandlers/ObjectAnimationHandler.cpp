#include "pch.h"
#include "ObjectAnimationHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool ObjectAnimationHandler::Handle(Net::IPacket* packet)
{
    ObjectAnimationPacket* received_packet = dynamic_cast<ObjectAnimationPacket*>(packet);
    if (!received_packet) return false;

    std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_id);
    if (!IsValid(network_actor)) return false;

    network_actor->ReceivePacket(received_packet);
    return true;
}

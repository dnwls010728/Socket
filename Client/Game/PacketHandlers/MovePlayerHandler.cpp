#include "pch.h"
#include "MovePlayerHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool MovePlayerHandler::Handle(Net::IPacket* packet)
{
    MovePlayerPacket* received_packet = dynamic_cast<MovePlayerPacket*>(packet);
    if (!received_packet) return false;

    std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->unique_id);
    if (IsValid(network_actor)) network_actor->ReceivePacket(packet);
    
    return true;
}

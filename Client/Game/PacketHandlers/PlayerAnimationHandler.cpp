#include "pch.h"
#include "PlayerAnimationHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool PlayerAnimationHandler::Handle(Net::IPacket* packet)
{
    PlayerAnimationPacket* received_packet = dynamic_cast<PlayerAnimationPacket*>(packet);
    if (!received_packet) return false;

    std::shared_ptr<NetworkActor> network_actor = NetworkSubsystem::Get()->FindNetworkActor(received_packet->unique_id);
    if (IsValid(network_actor)) network_actor->ReceivePacket(packet);
    
    return true;
}

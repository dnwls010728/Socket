#include "pch.h"
#include "TakeDamageHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Actors/Interfaces/IDamageable.h"
#include "Subsystems/NetworkSubsystem.h"

bool TakeDamageHandler::Handle(Net::IPacket* packet)
{
    TakeDamagePacket* received_packet = dynamic_cast<TakeDamagePacket*>(packet);
    if (!received_packet) return false;

    NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();
    std::shared_ptr<NetworkActor> network_actor = network_subsystem->FindNetworkActor(received_packet->object_id);
    if (network_actor)
    {
        if (auto damageable = std::dynamic_pointer_cast<IDamageable>(network_actor))
        {
            damageable->TakeDamage(received_packet->damage_amount, received_packet->server_time);
        }
    }
    
    return true;
}

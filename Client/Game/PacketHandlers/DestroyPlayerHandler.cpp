#include "pch.h"
#include "DestroyPlayerHandler.h"

#include <CustomPacket.h>

#include "Subsystems/NetworkSubsystem.h"

bool DestroyPlayerHandler::Handle(Net::IPacket* packet)
{
    DestroyPlayerPacket* received_packet = static_cast<DestroyPlayerPacket*>(packet);
    if (!received_packet) return false;

    NetworkSubsystem::Get()->DestroyNetworkActor(received_packet->unique_id);
    return true;
}

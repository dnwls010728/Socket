#include "pch.h"
#include "MapSetupHandler.h"

#include <CustomPacket.h>

#include "Subsystems/NetworkSubsystem.h"

bool MapSetupHandler::Handle(Net::IPacket* packet)
{
    MapSetupPacket* received_packet = dynamic_cast<MapSetupPacket*>(packet);
    if (!received_packet) return false;

    NetworkSubsystem::Get()->SetupMap(received_packet->map_id);

    return true;
}

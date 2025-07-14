#include "pch.h"
#include "MapSetupHandler.h"

#include <CustomPacket.h>

#include "Subsystems/NetworkSubsystem.h"

bool MapSetupHandler::Handle(Net::IPacket* packet)
{
    MapSetupPacket* received_packet = dynamic_cast<MapSetupPacket*>(packet);
    if (!received_packet) return false;

    Math::Vector2 spawn_position = { received_packet->position_x, received_packet->position_y };
    NetworkSubsystem::Get()->SetupMap(received_packet->map_id, spawn_position);

    return true;
}

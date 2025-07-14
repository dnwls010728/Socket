#include "pch.h"
#include "MapLoadHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool MapLoadHandler::Handle(Net::IPacket* packet)
{
    MapLoadPacket* received_packet = dynamic_cast<MapLoadPacket*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();
    player_subsystem->map_id_ = received_packet->map_id;
    player_subsystem->spawn_position.x = received_packet->spawn_position.x;
    player_subsystem->spawn_position.y = received_packet->spawn_position.y;

    World::Get()->OpenLevel(L"Game");
    return true;
}

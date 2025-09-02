#include "pch.h"
#include "PlacementStartHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlacementSubsystem.h"

bool PlacementStartHandler::Handle(Net::IPacket* packet)
{
    PlacementStartPacket* received_packet = dynamic_cast<PlacementStartPacket*>(packet);
    if (!received_packet) return false;

    PlacementSubsystem::Get()->StartPlacement();

    return true;
}

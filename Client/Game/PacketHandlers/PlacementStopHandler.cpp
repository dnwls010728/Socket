#include "pch.h"
#include "PlacementStopHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlacementSubsystem.h"

bool PlacementStopHandler::Handle(Net::IPacket* packet)
{
    PlacementStopResponse* received_packet = dynamic_cast<PlacementStopResponse*>(packet);
    if (!received_packet) return false;

    PlacementSubsystem::Get()->StopPlacement();

    return true;
}

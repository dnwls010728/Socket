#include "pch.h"
#include "PickupItemHandler.h"

#include <CustomPacket.h>

#include "Subsystems/NetworkSubsystem.h"

bool PickupItemHandler::Handle(Net::IPacket* packet)
{
    PickupItemResponse* received_packet = dynamic_cast<PickupItemResponse*>(packet);
    if (!received_packet) return false;

    return true;
}

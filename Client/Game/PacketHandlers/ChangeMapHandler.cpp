#include "pch.h"
#include "ChangeMapHandler.h"

#include <CustomPacket.h>

#include "Subsystems/NetworkSubsystem.h"

bool ChangeMapHandler::Handle(Net::IPacket* packet)
{
    ChangeMapResponse* response = dynamic_cast<ChangeMapResponse*>(packet);
    if (!response || !response->is_success) return false;

    NetworkSubsystem::Get()->SetupMap(response->map_id);
    return true;
}

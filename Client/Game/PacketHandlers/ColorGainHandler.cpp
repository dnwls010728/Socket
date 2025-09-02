#include "pch.h"
#include "ColorGainHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool ColorGainHandler::Handle(Net::IPacket* packet)
{
    ColorGainPacket* received_packet = dynamic_cast<ColorGainPacket*>(packet);
    if (!received_packet) return false;
    
    OLD_Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    inventory->SetColor(received_packet->color);

    return true;
}

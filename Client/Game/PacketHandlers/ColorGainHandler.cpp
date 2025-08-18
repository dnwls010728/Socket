#include "pch.h"
#include "ColorGainHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool ColorGainHandler::Handle(Net::IPacket* packet)
{
    ColorGainPacket* received_packet = dynamic_cast<ColorGainPacket*>(packet);
    if (!received_packet) return false;
    
    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    inventory->SetColor(received_packet->color);

    Logger::Print(L"Color: %d", received_packet->color);

    return true;
}

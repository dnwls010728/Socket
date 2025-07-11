#include "pch.h"
#include "DropItemHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"

bool DropItemHandler::Handle(Net::IPacket* packet)
{
    DropItemResponse* received_packet = dynamic_cast<DropItemResponse*>(packet);
    if (!received_packet) return false;

    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    if (received_packet->count > 0)
    {
        inventory->ChangeCount(received_packet->slot_id, received_packet->count);
    }
    else
    {
        inventory->Remove(received_packet->slot_id);
    }
    
    return true;
}

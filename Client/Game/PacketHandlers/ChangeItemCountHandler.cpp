#include "pch.h"
#include "ChangeItemCountHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"

bool ChangeItemCountHandler::Handle(Net::IPacket* packet)
{
    ChangeItemCountPacket* received_packet = dynamic_cast<ChangeItemCountPacket*>(packet);
    if (!received_packet) return false;
    
    Inventory::Type inventory_type = static_cast<Inventory::Type>(received_packet->inventory_type);

    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    inventory->ChangeCount(inventory_type, received_packet->slot_index, received_packet->count);

    return true;
}

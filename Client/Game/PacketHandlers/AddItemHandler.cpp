#include "pch.h"
#include "AddItemHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"

bool AddItemHandler::Handle(Net::IPacket* packet)
{
    AddItemPacket* received_packet = dynamic_cast<AddItemPacket*>(packet);
    if (!received_packet) return false;

    Inventory::Type inventory_type = static_cast<Inventory::Type>(received_packet->inventory_type);

    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    inventory->AddSlot(inventory_type, received_packet->slot_index, received_packet->item_id, received_packet->count);

    return true;
}

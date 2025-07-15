#include "pch.h"
#include "MoveItemHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"

bool MoveItemHandler::Handle(Net::IPacket* packet)
{
    MoveItemResponse* response = dynamic_cast<MoveItemResponse*>(packet);
    if (!response) return false;

    Inventory::Type inventory_type = static_cast<Inventory::Type>(response->inventory_type);

    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
    inventory->Swap(inventory_type, response->first_slot, inventory_type, response->second_slot);
    
    return true;
}

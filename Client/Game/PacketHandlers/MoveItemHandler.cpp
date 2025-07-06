#include "pch.h"
#include "MoveItemHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"

bool MoveItemHandler::Handle(Net::IPacket* packet)
{
    MoveItemResponse* response = dynamic_cast<MoveItemResponse*>(packet);
    if (!response) return false;

    const std::vector<InventoryChange>& changes = response->changes;
    if (!changes.empty())
    {
        Inventory* inventory = PlayerSubsystem::Get()->GetInventory();
        inventory->Swap(changes[0].dest, changes[0].arg);
    }
    
    return true;
}

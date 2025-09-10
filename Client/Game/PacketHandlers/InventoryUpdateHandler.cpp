#include "pch.h"
#include "InventoryUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool InventoryUpdateHandler::Handle(Net::IPacket* packet)
{
    InventoryUpdatePacket* received_packet = static_cast<InventoryUpdatePacket*>(packet);
    if (!received_packet) return false;
    
    Inventory* inventory = PlayerSubsystem::Get()->GetInventory();

    const auto& changes = received_packet->changes;
    for (const auto& change : changes)
    {
        InventoryType inventory_type = static_cast<InventoryType>(change.inventory_type);
        
        switch (change.action)
        {
        case InventoryAction::kAdd:
            {
                inventory->AddSlot(inventory_type, change.add.slot_id, change.add.item_id, change.add.count);
            }
            break;

        case InventoryAction::kChangeCount:
            {
                inventory->ChangeCount(inventory_type, change.change_count.slot_id, change.change_count.count);
            }
            break;

        case InventoryAction::kMove:
            {
                inventory->Swap(inventory_type, change.move.first_slot, inventory_type, change.move.second_slot);
            }
            break;

        case InventoryAction::kRemove:
            {
                inventory->Remove(inventory_type, change.remove.slot_id);
            }
            break;
        }
    }

    return true;
}

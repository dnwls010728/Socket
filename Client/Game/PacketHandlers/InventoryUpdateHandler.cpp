#include "pch.h"
#include "InventoryUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool InventoryUpdateHandler::Handle(Net::IPacket* packet)
{
    InventoryUpdatePacket* received_packet = static_cast<InventoryUpdatePacket*>(packet);
    if (!received_packet) return false;
    
    OLD_Inventory* inventory = PlayerSubsystem::Get()->GetInventory();

    const auto& changes = received_packet->changes;
    for (const auto& change : changes)
    {
        OLD_Inventory::Type inventory_type = static_cast<OLD_Inventory::Type>(change.inventory_type);
        
        switch (change.action)
        {
        case InventoryAction::kAdd:
            {
                inventory->AddSlot(inventory_type, change.info.add.slot_id, change.info.add.item_id, change.info.add.count);
            }
            break;

        case InventoryAction::kChangeCount:
            {
                inventory->ChangeCount(inventory_type, change.info.change_count.slot_id, change.info.change_count.count);
            }
            break;

        case InventoryAction::kMove:
            {
                inventory->Swap(inventory_type, change.info.move.first_slot, inventory_type, change.info.move.second_slot);
            }
            break;

        case InventoryAction::kRemove:
            {
                inventory->Remove(inventory_type, change.info.remove.slot_id);
            }
            break;
        }
    }

    return true;
}

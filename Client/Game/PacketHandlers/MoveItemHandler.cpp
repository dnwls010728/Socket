#include "pch.h"
#include "MoveItemHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"
#include "UI/UIState.h"
#include "UI/Element/Inventory/UIInventory.h"

bool MoveItemHandler::Handle(Net::IPacket* packet)
{
    MoveItemResponse* response = dynamic_cast<MoveItemResponse*>(packet);
    if (!response) return false;

    if (response->changes.size() > 0)
    {
        PlayerSubsystem::Get()->GetInventory()->Swap(response->changes[0].dest, response->changes[0].arg);

        UIInventory* ui_inventory = UI::Get()->GetState()->FindElement<UIInventory>(L"Inventory");
        if (ui_inventory) ui_inventory->RefreshSlots();
    }
    
    return true;
}

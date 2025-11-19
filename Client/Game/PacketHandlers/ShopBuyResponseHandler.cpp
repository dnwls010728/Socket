#include "pch.h"
#include "ShopBuyResponseHandler.h"

#include <CustomPacket.h>

#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/Shop/UIShop.h"

bool ShopBuyResponseHandler::Handle(Net::IPacket* packet)
{
    auto* response = dynamic_cast<ShopBuyResponse*>(packet);
    if (!response) return false;

    if (response->success)
    {
        if (auto* inventory = PlayerSubsystem::Get()->GetInventory())
        {
            inventory->SetColor(response->color);
        }
    }

    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        if (auto* shop = state->GetShop())
        {
            shop->HandleBuyResponse(*response);
        }
    }

    return true;
}

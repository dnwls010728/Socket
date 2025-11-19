#include "pch.h"
#include "ShopSellPriceResponseHandler.h"

#include <CustomPacket.h>

#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/Shop/UIShop.h"

bool ShopSellPriceResponseHandler::Handle(Net::IPacket* packet)
{
    auto* response = dynamic_cast<ShopSellPriceResponse*>(packet);
    if (!response) return false;

    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        if (auto* shop = state->GetShop())
        {
            shop->HandleSellPriceResponse(*response);
        }
    }

    return true;
}

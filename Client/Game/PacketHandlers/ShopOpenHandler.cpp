#include "pch.h"
#include "ShopOpenHandler.h"

#include <CustomPacket.h>

#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/Shop/UIShop.h"

bool ShopOpenHandler::Handle(Net::IPacket* packet)
{
    ShopOpenResponse* response = dynamic_cast<ShopOpenResponse*>(packet);
    if (!response) return false;
    UIInGameState* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
    if (!state) return false;

    if (auto* shop = state->GetShop())
    {
        shop->OpenShop(response->npc_id, response->items);
        return true;
    }

    return false;
}

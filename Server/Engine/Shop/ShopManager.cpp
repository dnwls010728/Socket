#include "pch.h"
#include "ShopManager.h"

#include "Shop.h"

ShopManager::ShopManager() :
    shops_(),
    npc_shops_()
{
}

std::shared_ptr<Shop> ShopManager::GetShop(int32_t id)
{
    auto it = shops_.find(id);
    if (it != shops_.end()) return nullptr;
    return LoadShop(id, true);
}

std::shared_ptr<Shop> ShopManager::GetShopByNPC(int32_t npc_id)
{
    auto it = npc_shops_.find(npc_id);
    if (it != npc_shops_.end()) return nullptr;
    return LoadShop(npc_id, false);
}

std::shared_ptr<Shop> ShopManager::LoadShop(int32_t id, bool is_shop_id)
{
    std::shared_ptr<Shop> shop = Shop::CreateShop(id, is_shop_id);
    if (shop)
    {
        shops_[shop->GetID()] = shop;
        npc_shops_[shop->GetNPCID()] = shop;
    }
    else if (is_shop_id) shops_[id] = nullptr;
    else npc_shops_[id] = nullptr;
    
    return shop;
}

#include "pch.h"
#include "ShopManager.h"

#include "Shop.h"

ShopManager::ShopManager() :
    shops_()
{
}

std::shared_ptr<Shop> ShopManager::GetShop(int32_t id)
{
    auto it = shops_.find(id);
    if (it != shops_.end()) return nullptr;
    return LoadShop(id);
}

std::shared_ptr<Shop> ShopManager::LoadShop(int32_t id)
{
    std::shared_ptr<Shop> shop = Shop::CreateShop(id);
    if (!shop) return nullptr;
    return shop;
}

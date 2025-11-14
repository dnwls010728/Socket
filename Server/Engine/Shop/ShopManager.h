#pragma once
#include <unordered_map>

#include "Singleton.h"

class Shop;

class ShopManager : public Singleton<ShopManager>
{
public:
    ShopManager();
    virtual ~ShopManager() override = default;

    std::shared_ptr<Shop> GetShop(int32_t id);
    std::shared_ptr<Shop> GetShopByNPC(int32_t npc_id);

private:
    std::shared_ptr<Shop> LoadShop(int32_t id, bool is_shop_id);
    
    std::unordered_map<int32_t, std::shared_ptr<Shop>> shops_;
    std::unordered_map<int32_t, std::shared_ptr<Shop>> npc_shops_;
    
};

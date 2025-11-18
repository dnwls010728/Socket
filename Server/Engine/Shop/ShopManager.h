#pragma once
#include <unordered_map>

#include "Singleton.h"

class Shop;

class ShopManager : public Singleton<ShopManager>
{
public:
    ShopManager();
    virtual ~ShopManager() override = default;

    std::shared_ptr<Shop> GetShop(uint32_t id);
    std::shared_ptr<Shop> GetShopByNPC(uint32_t npc_id);

private:
    std::shared_ptr<Shop> LoadShop(uint32_t id, bool is_shop_id);
    
    std::unordered_map<uint32_t, std::shared_ptr<Shop>> shops_;
    std::unordered_map<uint32_t, std::shared_ptr<Shop>> npc_shops_;
    
};

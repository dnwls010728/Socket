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

private:
    std::shared_ptr<Shop> LoadShop(int32_t id);
    
    std::unordered_map<int32_t, std::shared_ptr<Shop>> shops_;
    
};

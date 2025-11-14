#pragma once
#include <cstdint>
#include <memory>
#include <vector>

class PlayerCharacter;
class ShopItem;

class Shop
{
public:
    Shop(int32_t id, int32_t npc_id);
    ~Shop() = default;

    void AddItem(const std::shared_ptr<ShopItem>& item);
    void SendShop(const std::shared_ptr<PlayerCharacter>& player);

    static std::shared_ptr<Shop> CreateShop(int32_t id);

private:
    int32_t id_;
    int32_t npc_id_;
    
    std::vector<std::shared_ptr<ShopItem>> items_;
    
};

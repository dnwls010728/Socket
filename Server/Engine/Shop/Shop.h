#pragma once
#include <cstdint>
#include <memory>
#include <vector>

class PlayerCharacter;
class ShopItem;

class Shop
{
public:
    Shop(uint32_t id, uint32_t npc_id);
    ~Shop() = default;

    void AddItem(const std::shared_ptr<ShopItem>& item);
    void SendShop(const std::shared_ptr<PlayerCharacter>& player);
    
    inline uint32_t GetID() const { return id_; }
    inline uint32_t GetNPCID() const { return npc_id_; }

    static std::shared_ptr<Shop> CreateShop(uint32_t id, bool is_shop_id);

private:
    uint32_t id_;
    uint32_t npc_id_;
    
    std::vector<std::shared_ptr<ShopItem>> items_;
    
};

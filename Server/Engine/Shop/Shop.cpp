#include "pch.h"
#include "Shop.h"

#include <CustomPacket.h>

#include "ShopItem.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "MySQL/MySQLManager.h"

Shop::Shop(uint32_t id, uint32_t npc_id) :
    id_(id),
    npc_id_(npc_id),
    items_()
{
}

void Shop::AddItem(const std::shared_ptr<ShopItem>& item)
{
    items_.push_back(item);
}

void Shop::SendShop(const std::shared_ptr<PlayerCharacter>& player)
{
    player->SetShop(this);
    
    ShopOpenResponse packet;
    packet.npc_id = npc_id_;
    
    for (const auto& item : items_)
    {
        ShopItemInfo item_info;
        item_info.item_id = item->GetItemID();
        item_info.price = item->GetPrice();
        packet.items.push_back(item_info);
    }
    
    player->SendPacket(packet);
}

void Shop::Sell(const std::shared_ptr<PlayerCharacter>& player)
{
}

std::shared_ptr<ShopItem> Shop::FindItem(uint32_t item_id) const
{
    for (const auto& item : items_)
    {
        if (item->GetItemID() == item_id)
            return item;
    }

    return nullptr;
}

bool Shop::CalculateSellPrice(uint32_t item_id, int32_t count, int32_t &price) const
{
    auto item_data = DataManager::Get()->GetItem(item_id);
    if (!item_data)
        return false;
    price = item_data->price * count;
    return true;
}

std::shared_ptr<Shop> Shop::CreateShop(uint32_t id, bool is_shop_id)
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return nullptr;

    std::shared_ptr<Shop> shop = nullptr;

    try
    {
        std::unique_ptr<sql::PreparedStatement> statement;
        
        if (is_shop_id) statement.reset(connection->prepareStatement("SELECT * FROM shop_info WHERE id = ?"));
        else statement.reset(connection->prepareStatement("SELECT * FROM shop_info WHERE npc_id = ?"));
        
        statement->setInt(1, id);
        
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
        if (!result->next()) return nullptr;
        
        uint32_t shop_id = result->getInt("id");
        uint32_t npc_id = result->getInt("npc_id");
        shop = std::make_unique<Shop>(shop_id, npc_id);

        statement.reset(connection->prepareStatement("SELECT * FROM shop_item_info WHERE shop_id = ? ORDER BY item_order DESC"));
        statement->setInt(1, shop->GetID());

        result.reset(statement->executeQuery());
        while (result->next())
        {
            uint32_t item_id = result->getInt("item_id");
            uint32_t price = result->getInt("price");
            
            shop->AddItem(std::make_shared<ShopItem>(item_id, price));
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;

        return nullptr;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return nullptr;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return nullptr;
    }
    
    return shop;
}

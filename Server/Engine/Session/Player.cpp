#include "pch.h"
#include "Player.h"

#include <CustomPacket.h>

#include "DataManager.h"
#include "IPacket.h"
#include "Session.h"
#include "../Map/World.h"
#include "../MySQL/MySQLManager.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "Player/Inventory/Inventory.h"

Player::Player(Session* session, uint32_t account_id) :
    session_(session),
    account_id_(account_id),
    player_character_(nullptr)
{
}

Player::~Player()
{
}

void Player::SendPacket(const Net::IPacket& packet) const
{
    if (!session_) return;
    session_->SendPacket(packet);
}

void Player::ReceivePacket(Net::IPacket* packet)
{
    switch (packet->GetPacketID())
    {
    case SelectCharacterRequest::StaticPacketID:
        {
            SelectCharacterRequest* request = static_cast<SelectCharacterRequest*>(packet);
            player_character_ = PlayerCharacter::LoadCharacter(request->unique_id, shared_from_this());
            
            SelectCharacterResponse response;
            response.name = player_character_->name_;
            response.character_color = player_character_->character_color_;
            response.character_id = player_character_->object_id_;
            response.lv = player_character_->lv_;
            response.hp = player_character_->hp_;
            response.max_hp = player_character_->max_hp_;
            response.exp = player_character_->exp_;
            response.color = player_character_->color_;
            response.map_id = player_character_->map_->GetMapID();
            response.spawn_position.x = player_character_->position_.x;
            response.spawn_position.y = player_character_->position_.y;

            const auto& inventories = player_character_->GetInventory()->GetInventories();
            for (const auto& inventory : inventories)
            {
                Inventory::Type type = inventory.first;
                for (const auto& slot : inventory.second)
                {
                    ItemInfo item_info;
                    item_info.inventory_type = static_cast<uint8_t>(type);
                    item_info.item_id = slot.second.item_id;
                    item_info.slot_index = slot.first;
                    item_info.count = slot.second.count;
                    response.inventory.push_back(item_info);
                }
            }

            SendPacket(response);

            session_->SetState(Session::State::kCharacterSelected);
        }
        break;
        
    default:
        break;
    }

    if (player_character_) player_character_->ReceivePacket(packet);
}

void Player::Update()
{
    if (player_character_) player_character_->UpdateCharacter();
}

void Player::ExitMap()
{
    if (player_character_)
    {
        player_character_->ExitMap();
        player_character_ = nullptr;
    }
}

bool Player::Disconnect()
{
    return session_->Disconnect();
}

std::vector<std::shared_ptr<PlayerCharacter>> Player::GetCharacters()
{
    std::vector<std::shared_ptr<PlayerCharacter>> characters;
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return {};
    
    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT character_id FROM character_info WHERE account_id = ?"));
            statement->setUInt(1, account_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                uint32_t character_id = result->getInt("character_id");
                auto character = PlayerCharacter::LoadCharacter(character_id, shared_from_this());
                if (character) characters.push_back(character);
            }
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;

        return {};
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return {};
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return {};
    }
    
    return characters;
}

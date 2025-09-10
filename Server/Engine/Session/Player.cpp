#include "pch.h"
#include "Player.h"

#include <CustomPacket.h>

#include "DataManager.h"
#include "IPacket.h"
#include "Session.h"
#include "../Map/World.h"
#include "../MySQL/MySQLManager.h"
#include "PartyManager.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "Player/Inventory/Item.h"

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
    case CheckNameRequest::StaticPacketID:
        {
            CheckNameRequest* request = static_cast<CheckNameRequest*>(packet);
            
            sql::Connection* connection = MySQLManager::Get()->GetConnection();
            if (!connection) break;

            std::string name = StringHelper::UTF16ToUTF8(request->name);

            try
            {
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM character_info WHERE name = ?"));
                statement->setString(1, name);

                std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
                
                CheckNameResponse response;
                
                if (result->next()) response.is_available = false;
                else response.is_available = true;
                
                SendPacket(response);
            }
            catch (sql::SQLException& e)
            {
                std::cerr << "SQLException: " << e.what() << std::endl;
                std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
                std::cerr << "SQL State: " << e.getSQLState() << std::endl;
                break;
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception: " << e.what() << std::endl;
                break;
            }
            catch (...)
            {
                std::cerr << "Unknown Exception" << std::endl;
                break;
            }
        }
        break;

    case CreateCharacterRequest::StaticPacketID:
        {
            CreateCharacterRequest* request = static_cast<CreateCharacterRequest*>(packet);

            std::shared_ptr<PlayerCharacter> new_character = PlayerCharacter::CreateCharacter(shared_from_this());
            new_character->SetName(request->name);
            new_character->SetBodyColor(request->body_color);
            new_character->SetMapID(1);
            
            sql::Connection* connection = MySQLManager::Get()->GetConnection();
            if (!connection) break;
            
            try
            {
                std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO character_info (account_id, name, body_color, lv, hp, max_hp, exp, map_id, last_position_x, last_position_y, color) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
                statement->setUInt(1, account_id_);
                statement->setString(2, StringHelper::UTF16ToUTF8(new_character->GetName()));
                statement->setString(3, StringHelper::UTF16ToUTF8(new_character->GetBodyColor()));
                statement->setInt(4, new_character->lv_);
                statement->setInt(5, new_character->hp_);
                statement->setInt(6, new_character->base_max_hp_);
                statement->setInt(7, new_character->exp_);
                statement->setInt(8, new_character->map_id_);
                statement->setDouble(9, new_character->position_.x);
                statement->setDouble(10, new_character->position_.y);
                statement->setInt(11, new_character->color_);
                statement->executeUpdate();

                statement.reset(connection->prepareStatement("SELECT LAST_INSERT_ID()"));
                std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
                if (result->next())
                {
                    uint32_t character_id = result->getUInt(1);
                    new_character->SetObjectID(character_id);
                }
            }
            catch (sql::SQLException& e)
            {
                std::cerr << "SQLException: " << e.what() << std::endl;
                std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
                std::cerr << "SQL State: " << e.getSQLState() << std::endl;
                break;
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception: " << e.what() << std::endl;
                break;
            }
            catch (...)
            {
                std::cerr << "Unknown Exception" << std::endl;
                break;
            }

            // if (!new_character->GetInventory()->UpdateDatabase()) break;

            CreateCharacterResponse response;
            response.profile.character_id = new_character->GetObjectID();
            response.profile.map_id = new_character->GetMapID();
            response.profile.name = new_character->GetName();
                        
            response.profile.position.x = new_character->GetPosition().x;
            response.profile.position.y = new_character->GetPosition().y;

            response.profile.body_color = new_character->GetBodyColor();

            response.profile.stats.hp = new_character->hp_;
            response.profile.stats.max_hp = new_character->base_max_hp_;
            response.profile.stats.exp = new_character->exp_;
            response.profile.stats.lv = new_character->lv_;
            SendPacket(response);
        }
        break;

    case DeleteCharacterRequest::StaticPacketID:
        {
            DeleteCharacterRequest* request = static_cast<DeleteCharacterRequest*>(packet);
            if (!PlayerCharacter::DeleteCharacter(request->character_id)) return;

            DeleteCharacterResponse response;
            response.character_id = request->character_id;
            SendPacket(response);
        }
        break;
        
    case SelectCharacterRequest::StaticPacketID:
        {
            SelectCharacterRequest* request = static_cast<SelectCharacterRequest*>(packet);
            player_character_ = PlayerCharacter::LoadCharacter(request->character_id, shared_from_this());
            
            SelectCharacterResponse response;
            response.name = player_character_->name_;
            response.body_color = player_character_->body_color_;
            response.character_id = player_character_->object_id_;
            response.lv = player_character_->lv_;
            response.hp = player_character_->hp_;
            response.max_hp = player_character_->effective_max_hp_;
            response.exp = player_character_->exp_;
            response.color = player_character_->color_;
            response.atk = player_character_->effective_atk_;
            response.def = player_character_->effective_def_;
            response.dig = player_character_->effective_dig_;
            response.map_id = player_character_->map_->GetMapID();
            response.spawn_position.x = player_character_->position_.x;
            response.spawn_position.y = player_character_->position_.y;

            auto* equip = player_character_->GetInventory(InventoryType::kEquip);
            auto* use = player_character_->GetInventory(InventoryType::kUse);
            auto* etc = player_character_->GetInventory(InventoryType::kEtc);
            auto* equipped = player_character_->GetInventory(InventoryType::kEquipped);

            response.equip_slot_capacity = equip->GetCapacity();
            response.use_slot_capacity = use->GetCapacity();
            response.etc_slot_capacity = etc->GetCapacity();

            for (const auto& slot : equip->GetItems())
            {
                const auto& item = slot.second;
                
                ItemInfo item_info;
                item_info.inventory_type = static_cast<uint8_t>(InventoryType::kEquip);
                item_info.item_id = item->GetID();
                item_info.slot_id = slot.first;
                item_info.count = item->GetCount();
                response.inventory.push_back(item_info);
            }

            for (const auto& slot : use->GetItems())
            {
                const auto& item = slot.second;
                
                ItemInfo item_info;
                item_info.inventory_type = static_cast<uint8_t>(InventoryType::kUse);
                item_info.item_id = item->GetID();
                item_info.slot_id = slot.first;
                item_info.count = item->GetCount();
                response.inventory.push_back(item_info);
            }

            for (const auto& slot : etc->GetItems())
            {
                const auto& item = slot.second;
                
                ItemInfo item_info;
                item_info.inventory_type = static_cast<uint8_t>(InventoryType::kEtc);
                item_info.item_id = item->GetID();
                item_info.slot_id = slot.first;
                item_info.count = item->GetCount();
                response.inventory.push_back(item_info);
            }

            for (const auto& slot : equipped->GetItems())
            {
                const auto& item = slot.second;
                
                ItemInfo item_info;
                item_info.inventory_type = static_cast<uint8_t>(InventoryType::kEquipped);
                item_info.item_id = item->GetID();
                item_info.slot_id = slot.first;
                item_info.count = item->GetCount();
                response.inventory.push_back(item_info);
            }

            SendPacket(response);

            if (player_character_ && player_character_->GetPartyID() != 0)
            {
                PartyManager::Get()->AddPlayerToParty(player_character_->GetPartyID(), shared_from_this());
            }

            session_->SetState(Session::State::kCharacterSelected);
        }
        break;
        
    default:
        break;
    }

    if (player_character_) player_character_->ReceivePacket(packet);
}

void Player::UpdateDatabase()
{
    if (player_character_) player_character_->UpdateDatabase();
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

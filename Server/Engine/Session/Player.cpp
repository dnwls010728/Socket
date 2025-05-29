#include "pch.h"
#include "Player.h"

#include <CustomPacket.h>
#include <ranges>

#include "IPacket.h"
#include "Session.h"
#include "../Helper/StringHelper.h"
#include "../Map/World.h"
#include "../MySQL/MySQLManager.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Player/Inventory/Inventory.h"

Player::Player(Session* session, uint32_t account_id) :
    session_(session),
    account_id_(account_id),
    character_id_(0),
    map_(nullptr),
    character_info_(),
    name_(L""),
    lv_(0),
    map_id_(0),
    color_(0),
    inventory_(nullptr),
    position_(Math::Vector2::Zero())
{
}

Player::~Player()
{
    if (map_) map_->RemovePlayer(GetCharacterID());
}

void Player::LoadCharacter(uint32_t unique_id)
{
    character_id_ = unique_id;
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return;

    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM character_info WHERE character_id = ?"));
            statement->setInt(1, unique_id);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                name_ = StringHelper::UTF8ToUTF16(result->getString("name"));
                lv_ = result->getInt("lv");
                map_id_ = result->getInt("map_id");
                position_.x = static_cast<float>(result->getDouble("last_position_x"));
                position_.y = static_cast<float>(result->getDouble("last_position_y"));
                color_ = result->getInt("color");
            }
        }

        inventory_ = std::make_unique<Inventory>(this);

        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM inventory_item_info WHERE character_id = ?"));
            statement->setInt(1, unique_id);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                uint32_t item_id = result->getInt("item_id");
                uint32_t slot_index = result->getInt("slot_index");
                uint32_t count = result->getInt("count");

                inventory_->AddSlot(slot_index, item_id, count);
            }
        }

        map_ = World::Get()->GetMap(map_id_);
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
    }
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
            LoadCharacter(request->unique_id);
            
            SelectCharacterResponse response;
            response.is_success = true;
            response.message = L"";
            response.name = name_;
            response.character_id = character_id_;
            response.lv = lv_;
            response.color = color_;
            response.position_x = position_.x;
            response.position_y = position_.y;

            for (const auto& it : inventory_->GetSlots())
            {
                ItemInfo item_info;
                item_info.item_id = it.second.item_id;
                item_info.slot_index = it.first;
                item_info.count = it.second.count;

                response.inventory.push_back(item_info);
            }

            SendPacket(response);

            session_->SetState(Session::State::kCharacterSelected);
        }
        break;

    case InGameReadyPacket::StaticPacketID:
        {
            session_->SetState(Session::State::kInGame);

            if (map_)
            {
                ChangeMapResponse response;
                response.is_success = true;
                response.map_id = character_info_.map_id;
                SendPacket(response);
                
                map_->AddPlayer(shared_from_this());

                SetPosition({character_info_.last_position_x, character_info_.last_position_y});
            }
        }
        break;

    case ChangeMapRequest::StaticPacketID:
        {
            ChangeMapRequest* request = static_cast<ChangeMapRequest*>(packet);
            if (map_)
            {
                map_->RemovePlayer(GetCharacterID());
                
                map_ = World::Get()->GetMap(request->map_id);
                if (map_)
                {
                    ChangeMapResponse response;
                    response.is_success = true;
                    response.map_id = request->map_id;
                    SendPacket(response);
                    
                    map_->AddPlayer(shared_from_this());

                    SetPosition(Math::Vector2::Zero());
                    break;
                }
            }

            ChangeMapResponse response;
            response.is_success = false;
            response.map_id = 0;
            SendPacket(response);
        }
        break;

    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet);
            if (map_)
            {
                float position_x = move_player_packet->movement.x;
                float position_y = move_player_packet->movement.y;
                
                SetPosition({position_x, position_y});
                
                MovePlayerPacket move_player_broadcast_packet;
                move_player_broadcast_packet.unique_id = character_id_;
                move_player_broadcast_packet.movement.x = position_x;
                move_player_broadcast_packet.movement.y = position_y;
                map_->SendPacket(move_player_broadcast_packet, shared_from_this());
            }
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet);
            if (map_)
            {
                ChatMessagePacket chat_message_broadcast_packet;
                chat_message_broadcast_packet.unique_id = character_id_;
                chat_message_broadcast_packet.message = chat_message_packet->message;
                map_->SendPacket(chat_message_broadcast_packet);
            }
        }
        break;
        
    case MoveItemRequest::StaticPacketID:
        {
            MoveItemRequest* request = static_cast<MoveItemRequest*>(packet);
            if (!inventory_) return;
            
            uint32_t src = request->src;
            uint32_t dest = request->dest;

            switch (request->type)
            {
            case ItemMoveType::kMove:
                {
                    if (!inventory_->GetItemID(src)) break;
                    inventory_->Swap(src, dest);

                    MoveItemResponse response;
                    response.changes.push_back({ src, dest });
                    SendPacket(response);
                }
                break;

            case ItemMoveType::kDrop:
                {
                    if (!inventory_->GetItemID(src)) break;
                    // inventory_->Remove(src);
                }
                break;
                
            }
        }
        break;
        
    default:
        break;
    }
}

void Player::Update()
{
    if (inventory_) inventory_->Update();
}

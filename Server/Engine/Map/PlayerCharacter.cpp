#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "DataManager.h"
#include "NetDef.h"
#include "World.h"
#include "Helper/StringHelper.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "MySQL/MySQLManager.h"
#include "Session/Player.h"

PlayerCharacter::PlayerCharacter() :
    player_(),
    account_id_(0),
    name_(L"Unknown"),
    lv_(0),
    hp_(0),
    max_hp_(0),
    map_id_(0),
    exp_(0),
    color_(0),
    inventory_(nullptr),
    is_invincible_()
{
}

PlayerCharacter::~PlayerCharacter()
{
    if (map_) map_->RemovePlayer(object_id_);
}

std::shared_ptr<PlayerCharacter> PlayerCharacter::LoadCharacter(uint32_t character_id, const std::shared_ptr<Player>& player)
{
    std::shared_ptr<PlayerCharacter> character = std::make_shared<PlayerCharacter>();
    
    character->object_id_ = character_id;
    character->player_ = player;
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return nullptr;
    
    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM character_info WHERE character_id = ?"));
            statement->setInt(1, character->object_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                character->account_id_ = result->getInt("account_id");
                character->name_ = StringHelper::UTF8ToUTF16(result->getString("name"));
                character->lv_ = result->getInt("lv");
                character->hp_ = result->getInt("hp");
                character->max_hp_ = result->getInt("max_hp");
                character->map_id_ = result->getInt("map_id");
                character->position_.x = static_cast<float>(result->getDouble("last_position_x"));
                character->position_.y = static_cast<float>(result->getDouble("last_position_y"));
                character->exp_.store(result->getInt("exp"));
                character->color_.store(result->getInt("color"));
            }
        }

        character->inventory_ = std::make_unique<Inventory>(character);

        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM inventory_item_info WHERE character_id = ?"));
            statement->setInt(1, character->object_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                uint32_t item_id = result->getInt("item_id");
                uint32_t slot_index = result->getInt("slot_index");
                uint32_t count = result->getInt("count");

                character->inventory_->AddSlot(slot_index, item_id, count);
            }
        }

        character->map_ = World::Get()->GetMap(character->map_id_);
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

    return character;
}

void PlayerCharacter::SendPacket(const Net::IPacket& packet) const
{
    if (auto player = player_.lock())
    {
        player->SendPacket(packet);
    }
}

void PlayerCharacter::ReceivePacket(Net::IPacket* packet)
{
    switch (packet->GetPacketID())
    {
    case InGameReadyPacket::StaticPacketID:
        {
            if (map_)
            {
                ChangeMapResponse response;
                response.is_success = true;
                response.map_id = map_id_;
                SendPacket(response);
                
                map_->AddPlayer(std::static_pointer_cast<PlayerCharacter>(shared_from_this()));

                SetPosition({position_.x, position_.y});
            }
        }
        break;

    case ChangeMapRequest::StaticPacketID:
        {
            ChangeMapRequest* request = static_cast<ChangeMapRequest*>(packet);
            if (map_)
            {
                map_->RemovePlayer(GetObjectID());
                
                map_ = World::Get()->GetMap(request->map_id);
                if (map_)
                {
                    ChangeMapResponse response;
                    response.is_success = true;
                    response.map_id = request->map_id;
                    SendPacket(response);
                    
                    map_->AddPlayer(std::static_pointer_cast<PlayerCharacter>(shared_from_this()));

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
                float position_x = move_player_packet->position_x;
                float position_y = move_player_packet->position_y;
                
                SetPosition({position_x, position_y});
                
                MovePlayerPacket move_player_broadcast_packet;
                move_player_broadcast_packet.unique_id = object_id_;
                move_player_broadcast_packet.position_x = position_x;
                move_player_broadcast_packet.position_y = position_y;
                move_player_broadcast_packet.velocity_x = move_player_packet->velocity_x;
                move_player_broadcast_packet.velocity_y = move_player_packet->velocity_y;
                move_player_broadcast_packet.server_time = Net::GetClientTime();
                move_player_broadcast_packet.time_update = move_player_packet->time_update;
                map_->SendPacket(move_player_broadcast_packet, std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
            }
        }
        break;

    case PlayerAnimationPacket::StaticPacketID:
        {
            PlayerAnimationPacket* player_animation_packet = static_cast<PlayerAnimationPacket*>(packet);
            if (map_)
            {
                PlayerAnimationPacket player_animation_broadcast_packet;
                player_animation_broadcast_packet.unique_id = object_id_;
                player_animation_broadcast_packet.server_time = Net::GetClientTime();
                player_animation_broadcast_packet.animation = player_animation_packet->animation;
                player_animation_broadcast_packet.is_flipped =  player_animation_packet->is_flipped;
                map_->SendPacket(player_animation_broadcast_packet, std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
            }
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet);
            if (map_)
            {
                ChatMessagePacket chat_message_broadcast_packet;
                chat_message_broadcast_packet.unique_id = object_id_;
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
            }
        }
        break;

    case DropItemRequest::StaticPacketID:
        {
            DropItemRequest* request = static_cast<DropItemRequest*>(packet);

            uint32_t count = inventory_->GetItemCount(request->slot_id);
            uint32_t remaining_count = 0;

            if (request->count >= count) inventory_->Remove(request->slot_id);
            else
            {
                remaining_count = count - request->count;
                inventory_->ChangeCount(request->slot_id, remaining_count);
            }

            DropItemResponse response;
            response.slot_id = request->slot_id;
            response.count = remaining_count;
            response.position_x = position_.x;
            response.position_y = position_.y;
            SendPacket(response);
        }
        break;

    case AttackRequest::StaticPacketID:
        {
            AttackRequest* attack_request = static_cast<AttackRequest*>(packet);
            if (!map_) return;

            map_->OnAttack(object_id_, attack_request->object_id);
        }
        break;
        
    default:
        break;
    }
}

void PlayerCharacter::TakeDamage(uint32_t damage_amount)
{
    if (hp_ <= 0 || is_invincible_) return;
    hp_ -= damage_amount;

    TakeDamagePacket packet;
    packet.object_id = object_id_;
    packet.updated_hp = hp_;
    packet.damage_amount = damage_amount;
    packet.server_time = Net::GetClientTime();
    map_->SendPacket(packet);

    is_invincible_.Set(2.f);
}

void PlayerCharacter::ExitMap()
{
    if (map_)
    {
        map_->RemovePlayer(object_id_);
        map_ = nullptr;
    }
}

void PlayerCharacter::UpdateCharacter()
{
    if (inventory_) inventory_->Update();
}

void PlayerCharacter::GainExp(uint32_t amount)
{
    if (lv_ >= 50) return;

    exp_.fetch_add(amount);

    while (exp_ > DataManager::Get()->GetExp(lv_))
    {
        exp_.fetch_sub(DataManager::Get()->GetExp(lv_));
        if (exp_ < 0) exp_.store(0);
        
        ++lv_;

        if (lv_ == 50)
        {
            exp_.store(0);
            break;
        }
    }

    PlayerStatsUpdatePacket packet;
    packet.flags |= static_cast<uint8_t>(PlayerStat::kExp);
    packet.flags |= static_cast<uint8_t>(PlayerStat::kLv);
    
    packet.stats[2] = exp_;
    packet.stats[3] = lv_;
    SendPacket(packet);
}

void PlayerCharacter::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    is_invincible_.Tick(delta_time);
    
}

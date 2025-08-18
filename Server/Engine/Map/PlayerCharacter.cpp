#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "DataManager.h"
#include "NetDef.h"
#include "World.h"
#include "Helper/StringHelper.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "MapObjects/DroppedItem.h"
#include "Math/Math.h"
#include "MySQL/MySQLManager.h"
#include "Session/PartyManager.h"
#include "Session/Party.h"
#include "Session/Player.h"

PlayerCharacter::PlayerCharacter() :
    player_(),
    account_id_(0),
    party_id_(0),
    name_(L"Unknown"),
    body_color_(L"FFFFFF"),
    map_id_(0),
    lv_(1),
    hp_(350),
    max_hp_(350),
    exp_(0),
    color_(0),
    inventory_(nullptr),
    is_invincible_(),
    dropped_item_mutex_()
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
                character->body_color_ = StringHelper::UTF8ToUTF16(result->getString("body_color"));
                character->lv_ = result->getInt("lv");
                character->hp_ = result->getInt("hp");
                character->max_hp_ = result->getInt("max_hp");
                character->exp_.store(result->getInt("exp"));
                character->map_id_ = result->getInt("map_id");
                character->position_.x = static_cast<float>(result->getDouble("last_position_x"));
                character->position_.y = static_cast<float>(result->getDouble("last_position_y"));
                character->color_.store(result->getInt("color"));
            }
        }

        character->inventory_ = std::make_unique<Inventory>(character);
        Inventory* inventory = character->inventory_.get();

        inventory->SetSlotCapacity(Inventory::Type::kEquip, 128);
        inventory->SetSlotCapacity(Inventory::Type::kUse, 128);
        inventory->SetSlotCapacity(Inventory::Type::kEtc, 128);

        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM inventory_item_info WHERE character_id = ?"));
            statement->setUInt(1, character->object_id_);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                uint32_t inventory_type = result->getUInt("inventory_type");
                uint32_t item_id = result->getInt("item_id");
                uint32_t slot_index = result->getInt("slot_index");
                
                int32_t count = result->getInt("count");

                Inventory::Type type = static_cast<Inventory::Type>(inventory_type);
                inventory->AddSlot(type, slot_index, item_id, count);
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

std::shared_ptr<PlayerCharacter> PlayerCharacter::CreateCharacter(const std::shared_ptr<Player>& player)
{
    std::shared_ptr<PlayerCharacter> character = std::make_shared<PlayerCharacter>();

    character->player_ = player;
    character->account_id_ = player->GetAccountID();
    character->inventory_ = std::make_unique<Inventory>(character);
    
    return character;
}

bool PlayerCharacter::DeleteCharacter(uint32_t character_id)
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return false;

    try
    {
        std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("DELETE FROM character_info WHERE character_id = ?"));
        statement->setUInt(1, character_id);
        statement->executeUpdate();
        
        statement.reset(connection->prepareStatement("DELETE FROM inventory_item_info WHERE character_id = ?"));
        statement->setUInt(1, character_id);
        statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return false;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return false;
    }
    
    return true;
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
    case ChangeMapPacket::StaticPacketID:
        {
            ChangeMapPacket* change_map_packet = static_cast<ChangeMapPacket*>(packet);
            
            map_->RemovePlayer(GetObjectID());
            map_ = World::Get()->GetMap(change_map_packet->map_id);

            // 추후 포탈 이용 시 포탈 위치로 이동하도록 수정 필요
            SetPosition(Math::Vector2::Zero());

            MapLoadPacket map_reset_packet;
            map_reset_packet.map_id = map_->GetMapID();
            map_reset_packet.spawn_position.x = GetPosition().x;
            map_reset_packet.spawn_position.y = GetPosition().y;
            SendPacket(map_reset_packet);
        }
        break;
        
    case MapLoadCompletePacket::StaticPacketID:
        {
            map_->AddPlayer(std::static_pointer_cast<PlayerCharacter>(shared_from_this()));
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

            Inventory::Type inventory_type = static_cast<Inventory::Type>(request->inventory_type);
            
            if (!inventory_->GetItemID(inventory_type, request->first_slot)) break;
            inventory_->Swap(inventory_type, request->first_slot, inventory_type, request->second_slot);

            MoveItemResponse response;
            response.inventory_type = request->inventory_type;
            response.first_slot = request->first_slot;
            response.second_slot = request->second_slot;
            SendPacket(response);
        }
        break;

    case DropItemRequest::StaticPacketID:
        {
            DropItemRequest* request = static_cast<DropItemRequest*>(packet);

            Inventory::Type inventory_type = static_cast<Inventory::Type>(request->inventory_type);
            
            uint32_t item_id = inventory_->GetItemID(inventory_type, request->slot_index);
            
            int32_t count = inventory_->GetItemCount(inventory_type, request->slot_index);
            int32_t remaining_count = 0;
            
            if (request->count >= count) inventory_->Remove(inventory_type, request->slot_index);
            else
            {
                remaining_count = count - request->count;
                inventory_->ChangeCount(inventory_type, request->slot_index, remaining_count);
            }

            DropItemResponse response;
            response.inventory_type = request->inventory_type;
            response.slot_index = request->slot_index;
            response.count = remaining_count;
            SendPacket(response);
            
            Math::Vector2 drop_position = GetPosition();
            map_->GetDropPosition(drop_position);
            
            map_->SpawnItemDrop(item_id, request->count, std::static_pointer_cast<PlayerCharacter>(shared_from_this()), drop_position);
        }
        break;

    case PickupItemPacket::StaticPacketID:
        {
            PickupItemPacket* request = static_cast<PickupItemPacket*>(packet);

            std::shared_ptr<MapObject> map_object = map_->FindMapObject(request->object_id);
            if (!map_object) return;
            
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(map_object))
            {
                std::lock_guard<std::mutex> lock(dropped_item_mutex_);
                auto item = dropped_item->GetItem();
                if (inventory_->AddItem(item))
                {
                    map_->DestroyDroppedItem(dropped_item->GetObjectID(), object_id_);
                }
            }
        }
        break;

    case AttackRequest::StaticPacketID:
        {
            AttackRequest* attack_request = static_cast<AttackRequest*>(packet);
            if (!map_) return;

            map_->OnAttack(object_id_, attack_request->object_id);
        }
        break;
        
    case PartyInviteRequest::StaticPacketID:
        {
            PartyInviteRequest* request = static_cast<PartyInviteRequest*>(packet);
            if (!map_) break;

            auto sendPopup = [this](const std::wstring &msg)
            {
                PopupPacket popup_packet;
                popup_packet.text = msg;
                SendPacket(popup_packet);
            };
            
            if (GetPartyID() == 0)
            {
                sendPopup(L"초대할 파티가 존재하지 않습니다.");
                break;
            }

            const auto party = PartyManager::Get()->GetParty(GetPartyID());
            if (party == nullptr)
            {
                sendPopup(L"초대할 파티가 존재하지 않습니다.");
                break;
            }
            
            auto invitee = map_->FindPlayer(request->invitee_id);
            if (!invitee)
            {
                sendPopup(L"대상을 찾을 수 없습니다.");
                break;
            }

            if (invitee->GetPartyID() == party->GetPartyID())
            {
                sendPopup(L"같은 파티에 포함되어 있는 플레이어 입니다.");
                break;
            }
            
            if (invitee->GetPartyID() != 0)
            {
                sendPopup(L"이미 가입되어 있는 파티가 존재하는 플레이어 입니다.");
                break;
            }

            PartyInviteNotify notify;
            notify.inviter_name = GetName();
            notify.party_name = party->GetPartyName();
            notify.party_id = party->GetPartyID();
            invitee->SendPacket(notify);
        }
        break;

    case PartyInviteNotifyResponse::StaticPacketID:
        {
            PartyInviteNotifyResponse* response = static_cast<PartyInviteNotifyResponse*>(packet);
            if (!map_) break;

            if (response->result == false)
            {
                auto inviter = map_->FindPlayer(response->inviter_id);
                if (inviter)
                {
                    PopupPacket popup_packet;
                    popup_packet.text = GetName() + L" 님이 파티 초대를 거절했습니다.";
                    inviter->SendPacket(popup_packet);
                }
                break;
            }

            auto party = PartyManager::Get()->GetParty(response->party_id);
            if (!party)
            {
                PopupPacket popup_packet;
                popup_packet.text = L"존재하지 않는 파티입니다.";
                SendPacket(popup_packet);
                break;
            }

            if (party->GetPlayerCount() > 10)
            {
                PopupPacket popup_packet;
                popup_packet.text = L"파티 인원이 꽉찼습니다.";
                SendPacket(popup_packet);
                break;
            }
            
            PartyManager::Get()->AddPlayerToParty(party->GetPartyID(), player_.lock());
            SetPartyID(party->GetPartyID());

            PartyJoinPacket join_packet;
            join_packet.party_name = party->GetPartyName();
            join_packet.party_id = GetPartyID();
            join_packet.host_id = party->GetHost();
            SendPacket(join_packet);

            // 임시 알림
            PopupPacket join_msg;
            join_msg.text = GetName() + L" 님이 파티에 합류했습니다.";
            PartyManager::Get()->SendPacket(party->GetPartyID(), join_msg, GetAccountID());
            
        }
        break;

    case PartyCreateRequest::StaticPacketID:
        {
            PartyCreateRequest* request = static_cast<PartyCreateRequest*>(packet);
            auto party = PartyManager::Get()->CreateParty(request->party_name);
            if (!party)
            {
                break;
            }

            PartyManager::Get()->AddPlayerToParty(party->GetPartyID(), player_.lock());
            SetPartyID(party->GetPartyID());

            PopupPacket popup_packet;
            popup_packet.text = L"파티가 정상적으로 생성되었습니다.";
            SendPacket(popup_packet);

            PartyJoinPacket join_packet;
            join_packet.party_id = party->GetPartyID();
            join_packet.party_name = request->party_name;
            join_packet.host_id = GetAccountID();
            SendPacket(join_packet);
        }
        break;
        
    default:
        break;
    }
}

void PlayerCharacter::TakeDamage(int32_t damage_amount)
{
    if (hp_ <= 0 || is_invincible_) return;
    hp_ = std::clamp(hp_ - damage_amount, 0, max_hp_);

    TakeDamagePacket packet;
    packet.object_id = object_id_;
    packet.updated_hp = hp_;
    packet.damage_amount = damage_amount;
    packet.server_time = Net::GetClientTime();
    map_->SendPacket(packet);

    is_invincible_.Set(2.f);
}

bool PlayerCharacter::Disconnect()
{
    if (auto player = player_.lock())
    {
        player->Disconnect();
    }
    
    return false;
}

void PlayerCharacter::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);

    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kPlayer;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    PlayerInfo& info = packet.object_info.info.player;
    wcscpy_s(info.name, name_.c_str());
    wcscpy_s(info.body_color, body_color_.c_str());

    player->SendPacket(packet);
}

void PlayerCharacter::ExitMap()
{
    if (map_)
    {
        map_->RemovePlayer(object_id_);
        map_ = nullptr;
    }
}

void PlayerCharacter::UpdateDatabase()
{
    if (inventory_) inventory_->UpdateDatabase();
    
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return;
    
    try
    {
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("UPDATE character_info SET hp = ?, max_hp = ?, exp = ?, lv = ?, last_position_x = ?, last_position_y = ?, map_id = ? WHERE character_id = ?"));
            statement->setUInt(1, hp_);
            statement->setUInt(2, max_hp_);
            statement->setUInt(3, exp_);
            statement->setUInt(4, lv_);
            statement->setDouble(5, position_.x);
            statement->setDouble(6, position_.y);
            statement->setUInt(7, map_->GetMapID());
            statement->setUInt(8, object_id_);
            statement->executeUpdate();
        }
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

void PlayerCharacter::GainExp(int32_t amount)
{
    if (lv_ >= 50) return;

    exp_.fetch_add(amount);

    while (exp_ > DataManager::Get()->GetExp(lv_))
    {
        exp_.fetch_sub(DataManager::Get()->GetExp(lv_));
        if (exp_ < 0) exp_.store(0);
        
        ++lv_;

        max_hp_ += 25;
        hp_ = max_hp_;

        if (lv_ == 50)
        {
            exp_.store(0);
            break;
        }
    }

    PlayerStatsUpdatePacket packet;
    packet.stats[static_cast<uint8_t>(PlayerStat::kHP)] = hp_;
    packet.stats[static_cast<uint8_t>(PlayerStat::kMaxHP)] = max_hp_;
    packet.stats[static_cast<uint8_t>(PlayerStat::kExp)] = exp_;
    packet.stats[static_cast<uint8_t>(PlayerStat::kLv)] = lv_;
    SendPacket(packet);
}

void PlayerCharacter::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    is_invincible_.Tick(delta_time);
    
}

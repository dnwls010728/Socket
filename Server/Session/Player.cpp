#include "Player.h"

#include <CustomPacket.h>

#include "IPacket.h"
#include "Session.h"
#include "../Helper/StringHelper.h"
#include "../Map/MapManager.h"
#include "../MySQL/MySQLManager.h"

Player::Player(Session* session, uint32_t account_unique_id) :
    session_(session),
    account_unique_id_(account_unique_id),
    character_unique_id_(0),
    map_(nullptr),
    character_info_(),
    position_x_(0.f),
    position_y_(0.f)
{
}

Player::~Player()
{
    if (map_) map_->RemovePlayer(this);
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
            character_unique_id_ = request->unique_id;

            MySQLManager::Get()->ExecuteQuery(L"SELECT * FROM character_info WHERE unique_id = " + std::to_wstring(character_unique_id_), [&](const sql::ResultSet* result)
            {
                character_info_.unique_id = result->getInt("unique_id");
                character_info_.account_unique_id = result->getInt("account_unique_id");
                character_info_.name = StringHelper::ToWideString(result->getString("name"));
                character_info_.lv = result->getInt("lv");
                character_info_.job = result->getInt("job");
                character_info_.map_unique_id = result->getInt("map_unique_id");
                character_info_.last_position_x = static_cast<float>(result->getDouble("last_position_x"));
                character_info_.last_position_y = static_cast<float>(result->getDouble("last_position_y"));

                // 선택한 플레이어 정보를 클라이언트로 보내고, 클라이언트의 맵 전환
                SelectCharacterResponse response;
                response.is_success = true;
                response.message = L"Character selected successfully.";
                response.character_info = character_info_;
                SendPacket(response);

                map_ = MapManager::Get()->GetMap(character_info_.map_unique_id);
                // map_->AddPlayer(this);
            });
        }
        break;

    case ChangeMapRequest::StaticPacketID:
        {
            ChangeMapRequest* request = static_cast<ChangeMapRequest*>(packet);
            if (map_)
            {
                map_->RemovePlayer(this);
                map_ = MapManager::Get()->GetMap(request->map_unique_id);

                SetPosition(0.f, 0.f);

                ChangeMapResponse response;
                response.is_success = true;
                response.map_unique_id = request->map_unique_id;
                SendPacket(response);
                break;
            }

            ChangeMapResponse response;
            response.is_success = false;
            response.map_unique_id = 0;
            SendPacket(response);
        }
        break;

    case MapLoadCompletePacket::StaticPacketID:
        {
            map_->AddPlayer(this);
        }
        break;

    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet);
            if (map_)
            {
                float position_x = move_player_packet->movement.x;
                float position_y = move_player_packet->movement.y;
                
                SetPosition(position_x, position_y);
                
                MovePlayerPacket move_player_broadcast_packet;
                move_player_broadcast_packet.unique_id = character_unique_id_;
                move_player_broadcast_packet.movement.x = position_x;
                move_player_broadcast_packet.movement.y = position_y;
                map_->SendPacket(move_player_broadcast_packet, this);
            }
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet);
            if (map_)
            {
                ChatMessagePacket chat_message_broadcast_packet;
                chat_message_broadcast_packet.unique_id = character_unique_id_;
                chat_message_broadcast_packet.message = chat_message_packet->message;
                map_->SendPacket(chat_message_broadcast_packet);
            }
        }
        break;
        
    default:
        break;
    }
}

void Player::SetPosition(float x, float y)
{
    position_x_ = x;
    position_y_ = y;
}

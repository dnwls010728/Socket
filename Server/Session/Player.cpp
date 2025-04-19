#include "Player.h"

#include <CustomPacket.h>

#include "IPacket.h"
#include "Session.h"
#include "../Map/MapManager.h"
#include "../MySQL/MySQLManager.h"

Player::Player(Session* session, uint32_t account_unique_id) :
    session_(session),
    account_unique_id_(account_unique_id),
    character_unique_id_(0),
    map_(nullptr)
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
                GameMap* map = MapManager::Get()->GetMap(result->getInt("map"));
                if (map)
                {
                    map_ = map;
                    map_->AddPlayer(this);

                    ChangeMapPacket change_map_packet;
                    change_map_packet.map_id = map_->GetMapUniqueID();
                    SendPacket(change_map_packet);
                }
            });
        }
        break;
        
    default:
        break;
    }
}

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
                CharacterInfo character;
                character.unique_id = result->getInt("unique_id");
                character.account_unique_id = result->getInt("account_unique_id");
                character.name = StringHelper::ToWideString(result->getString("name"));
                character.lv = result->getInt("lv");
                character.job = result->getInt("job");
                character.map_unique_id = result->getInt("map_unique_id");
                character.last_position_x = static_cast<float>(result->getDouble("last_position_x"));
                character.last_position_y = static_cast<float>(result->getDouble("last_position_y"));

                // 선택한 플레이어 정보를 클라이언트로 보내고, 클라이언트의 맵 전환
                SelectCharacterResponse response;
                response.is_success = true;
                response.message = L"Character selected successfully.";
                response.character = character;
                SendPacket(response);

                map_ = MapManager::Get()->GetMap(character.map_unique_id);
                map_->AddPlayer(this);
            });
        }
        break;
        
    default:
        break;
    }
}

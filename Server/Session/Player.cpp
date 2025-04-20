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
                // TODO: HeavenMS - PlayerLoggedinHandler.java와 SetFieldHandler.cpp 참고
                CharacterInfo character;
                character.unique_id = result->getInt("unique_id");
                character.account_unique_id = result->getInt("account_unique_id");
                character.name = StringHelper::ToWideString(result->getString("name"));
                character.lv = result->getInt("lv");
                character.job = result->getInt("job");
                character.map = result->getInt("map");
                character.last_position_x = static_cast<float>(result->getDouble("last_position_x"));
                character.last_position_y = static_cast<float>(result->getDouble("last_position_y"));

                // 선택한 캐릭터 정보를 전송
                CharacterInfoPacket character_info_packet;
                character_info_packet.character = character;
                SendPacket(character_info_packet);
            });
        }
        break;
        
    default:
        break;
    }
}

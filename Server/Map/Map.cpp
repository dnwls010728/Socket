#include "Map.h"

#include <CustomPacket.h>

#include "../Session/Player.h"

Map::Map(uint32_t map_id) :
    mutex_(),
    map_unique_id_(map_id),
    map_object_unique_id_(100), // 플레이어 캐릭터 고유 ID로 인해, 100부터 시작
    map_objects_(),
    players_()
{
}

void Map::AddPlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(mutex_);
    players_.push_back(player);

    {
        // 맵에 플레이어가 추가되면, 다른 플레이어에게 스폰하도록 패킷을 전송
        SpawnPlayerPacket spawn_player_packet;
        spawn_player_packet.character_info = player->GetCharacterInfo();
        spawn_player_packet.position_x = player->GetPositionX();
        spawn_player_packet.position_y = player->GetPositionY();
        SendPacket(spawn_player_packet, player);
    }

    // 맵에 추가된 플레이어에게 다른 플레이어들을 스폰하도록 패킷을 전송
    for (auto& other_player : players_)
    {
        if (other_player && other_player != player)
        {
            SpawnPlayerPacket spawn_player_packet;
            spawn_player_packet.character_info = other_player->GetCharacterInfo();
            spawn_player_packet.position_x = other_player->GetPositionX();
            spawn_player_packet.position_y = other_player->GetPositionY();
            player->SendPacket(spawn_player_packet);
        }
    }
}

void Map::RemovePlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::erase(players_, player);

    {
        // 맵에서 플레이어가 제거되면, 다른 플레이어에게 제거하도록 패킷을 전송
        DestroyPlayerPacket destroy_player_packet;
        destroy_player_packet.unique_id = player->GetCharacterInfo().unique_id;
        SendPacket(destroy_player_packet, player);
    }
}

void Map::SendPacket(const Net::IPacket& packet)
{
    for (auto& player : players_)
    {
        if (player) player->SendPacket(packet);
    }
}

void Map::SendPacket(const Net::IPacket& packet, const Player* excluded_player)
{
    for (const auto& player : players_)
    {
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

uint32_t Map::GetMapObjectUniqueID()
{
    uint32_t unique_id = 0;

    do
    {
        unique_id = map_object_unique_id_.fetch_add(1);
    }
    while (map_objects_.contains(unique_id));

    return unique_id;
}

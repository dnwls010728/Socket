#include "GameMap.h"

#include <CustomPacket.h>
#include <iostream>
#include <ostream>

#include "../Session/Player.h"

GameMap::GameMap(uint32_t map_id) :
    mutex_(),
    map_unique_id_(map_id)
{
}

void GameMap::AddPlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(mutex_);
    players_.push_back(player);

    {
        // 맵에 플레이어가 추가되면, 다른 플레이어에게 스폰하도록 패킷을 전송
        SpawnPlayerPacket spawn_player_packet;
        SendPacket(spawn_player_packet, player);
    }

    // 맵에 추가된 플레이어에게 다른 플레이어들을 스폰하도록 패킷을 전송
    for (auto& other_player : players_)
    {
        if (other_player && other_player != player)
        {
            SpawnPlayerPacket spawn_player_packet;
            player->SendPacket(spawn_player_packet);
        }
    }
}

void GameMap::RemovePlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::erase(players_, player);

    {
        // 맵에서 플레이어가 제거되면, 다른 플레이어에게 제거하도록 패킷을 전송
        DestroyPlayerPacket destroy_player_packet;
        SendPacket(destroy_player_packet, player);
    }
}

void GameMap::SendPacket(const Net::IPacket& packet)
{
    for (auto& player : players_)
    {
        if (player) player->SendPacket(packet);
    }
}

void GameMap::SendPacket(const Net::IPacket& packet, const Player* excluded_player)
{
    for (const auto& player : players_)
    {
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

#include "GameMap.h"

#include <iostream>
#include <ostream>

#include "../Session/Player.h"

GameMap::GameMap(uint32_t map_id) :
    map_unique_id_(map_id)
{
}

void GameMap::AddPlayer(Player* player)
{
    players_.push_back(player);
    std::cout << map_unique_id_ << ": Add Player, Players: " << players_.size() << std::endl;
}

void GameMap::RemovePlayer(Player* player)
{
    std::erase(players_, player);
    std::cout << map_unique_id_ << ": Remove Player, Players: " << players_.size() << std::endl;
}

void GameMap::SendPacket(const Net::IPacket& packet)
{
    for (auto& player : players_)
    {
        if (player) player->SendPacket(packet);
    }
}

void GameMap::SendPacket(const Net::IPacket& packet, Player* excluded_player)
{
    for (auto& player : players_)
    {
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

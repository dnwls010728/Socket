#include "pch.h"
#include "Party.h"
#include "Player.h"
#include <ranges>

Party::Party(uint32_t party_id_) :
    party_id_(party_id_),
    host_id_(0)
{
}

void Party::SendPacket(const Net::IPacket& packet, uint32_t exclusion_player)
{
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& player : std::views::values(players_)) {
        if (player->GetAccountID() != exclusion_player )
            player->SendPacket(packet);
    }
}

bool Party::Contains(uint32_t player_id) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return players_.find(player_id) != players_.end();
}

bool Party::Contains(const std::shared_ptr<Player>& player) const
{
    if (!player) return false;
    return Contains(player->GetAccountID());
}

void Party::AddPlayer(const std::shared_ptr<Player>& player)
{
    if (!player) return;
    std::lock_guard<std::mutex> lock(mtx_);
    players_[player->GetAccountID()] = player;
}

void Party::RemovePlayer(uint32_t player_id)
{
    std::lock_guard<std::mutex> lock(mtx_);
    players_.erase(player_id);

    if (host_id_ == player_id)
    {
        if (players_.empty())
        {
            host_id_ = 0;
        }
        else
        {
            for (const auto& next_player_id : std::views::keys(players_))
            {
                host_id_ = next_player_id;
                
                // 호스트 변경 알림
            }
        }
    }
}

void Party::RemovePlayer(const std::shared_ptr<Player>& player)
{
    RemovePlayer(player->GetAccountID()); 
}

int Party::GetPlayerCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return static_cast<int>(players_.size());
}

std::vector<std::shared_ptr<Player>> Party::GetPlayers() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    
    std::vector<std::shared_ptr<Player>> players;
    for (const auto& player : players_)
    {
        players.push_back(player.second);
    }
    return players;
}
